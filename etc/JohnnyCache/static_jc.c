#include <linux/module.h>	/* Needed by all modules */
#include <linux/moduleparam.h>
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/mm.h>
#include <linux/rmap.h>
#include <linux/migrate.h>
#include <linux/hugetlb.h>
#include <linux/perf_event.h>
#include <asm-generic/io.h>
#include <linux/sort.h>
#include <linux/delay.h>

//Modification By OSM
#define CACHE_SIZE (96*512)
#define TOTAL_CACHE_SIZE (96*512) // DRAM is 64GB -> cache is 48GB on our machine -> 48*512 2MB pages

#define RING_SIZE (1000000) // Compute stats on the last XX memory access samples
#define CONTENTION_LEVELS 20 // Number of contention levels (to be in level N, a page must have been accessed heat_to_contention_level(N) times)

#define LARGE_PAGE_SIZE (2LU*1024*1024) // 2MB
#define pfn_to_large_pfn(pfn) ((pfn)/(512)) // 512 4KB pages in a 2MB page
#define pfn_to_bucket(pfn) (pfn_to_large_pfn(pfn) % CACHE_SIZE)
//Modification By OSM
#define test_pfn_to_bucket(pfn) (pfn_to_large_pfn(pfn) % TOTAL_CACHE_SIZE)

#define HEAT_PRINT_SIZE 2048
static char *printbuf;
static int total_allocated_pages = 0;

/* How hot is a cache region? */
static atomic_t heatmap[CACHE_SIZE];

/* Ring buffer of samples */
static struct ring {
   atomic_t idx;
   struct sample {
      int weight;
      u64 pfn;
   } samples[RING_SIZE];
} ring;

/* Metadata to help page allocations */
struct slot_list {
   size_t bucket;
   struct page *page;
   int heat;
   size_t used_by; // tgid
   size_t virt_addr;
   struct list_head contention_list;
   struct list_head bin_list;
};

static struct list_head unused_pages[CONTENTION_LEVELS]; // heat -> pages
static struct list_head allocated_pages[CONTENTION_LEVELS]; // heat -> pages
static struct list_head heated_pages[CONTENTION_LEVELS]; // heat -> pages
static struct list_head cache_bins[CACHE_SIZE]; // cache slot -> pages
static spinlock_t lock; // list_heads are not thread safe...


/*
 * Maintain heatmap
 * Maintain list of allocated and free pages at the correct contention level.
 * The current implementation maintains a ring buffer of samples, and updates the heatmap.
 *
 * E.g.,
 *  ring = [ pageX, pageY (current idx), pageZ ]
 *  When receiving a new sample, say on pageW, then
 *  ring = [ pageX, pageY, pageW (current idx) ]
 *  and the heat of pageZ is decreased and the heat of pageW is increased.
 *  The functions also update the heat of the cache slots.
 */
static size_t heat_to_contention_level(int heat) {
	/* Various definitions of contention */
   size_t idx = 0;
   idx = heat / 50;
   if(idx >= CONTENTION_LEVELS)
      idx = CONTENTION_LEVELS - 1;
   return idx;
}

/* Make sure all pages are in the correct contention list so that we can choose one from the lowest */
static void move_pages_contention(size_t bucket, int heat) {
   struct slot_list *s;
   size_t idx = heat_to_contention_level(heat);

   spin_lock(&lock);
   list_for_each_entry(s, &cache_bins[bucket], bin_list) {
      if(s->used_by && s->heat)
         list_move(&s->contention_list, &heated_pages[idx]);
      else if(s->used_by)
         list_move(&s->contention_list, &allocated_pages[idx]);
      else
         list_move(&s->contention_list, &unused_pages[idx]);
   }
   spin_unlock(&lock);
}

/*
 * At initialization time, add a new discovered 2MB page
 */
static int add_new_page(struct page *page) {
   //Modificaiton By OSM
   size_t test_bucket = test_pfn_to_bucket(page_to_pfn(page));

   size_t bucket = pfn_to_bucket(page_to_pfn(page));
   size_t idx = heat_to_contention_level(atomic_read(&heatmap[bucket]));
   struct slot_list *s = kmalloc(sizeof(*s), GFP_KERNEL);
   
   //Modificaiton By OSM
   if(CACHE_SIZE  <= test_bucket ) {
      printk(KERN_INFO "Fail to allocate by OSM\n");
      return 0;
   }
   
   if(!s) {
      printk(KERN_INFO "Fail to allocate all the pages!\n");
      return -1;
   }

   s->bucket = bucket;
   s->page = page;
   s->used_by = 0;
   s->heat = 0;
   INIT_LIST_HEAD(&s->bin_list);
   INIT_LIST_HEAD(&s->contention_list);
   list_add(&s->bin_list, &cache_bins[bucket]);
   list_add(&s->contention_list, &unused_pages[idx]);
   return 0;
}

/*
 * After munmap, re-add the page in the free list
 */
static int add_freed_page(struct page *page) {
   struct slot_list *s;
   size_t bucket = pfn_to_bucket(page_to_pfn(page));

   atomic_sub(50, &heatmap[bucket]);
   move_pages_contention(bucket, atomic_read(&heatmap[bucket]));

   {
      int heat = atomic_read(&heatmap[bucket]);
      int found = 0;
      size_t idx = heat_to_contention_level(heat);

      spin_lock(&lock);
      list_for_each_entry(s, &cache_bins[bucket], bin_list) {
         if(s->page == page) {
            s->used_by = 0;
            list_move(&s->contention_list, &unused_pages[idx]);
            found = 1;
            //printk("Freeing page %lu %lx\n", pfn_to_large_pfn(page_to_pfn(page)), s->virt_addr);
            break;
         }
      }
      spin_unlock(&lock);

      if(!found)
         printk("Didn't find page %lu\n", pfn_to_large_pfn(page_to_pfn(page)));
   }
   return 0;
}

/*
 * Mark a 2MB page as used (done just after mmap)
 */
//Modification By OSM
//static void reserve_page(struct hstate *h, int nid, pid_t pid, struct slot_list *s, int contention_idx, struct vm_area_struct *vma, unsigned long addr)
static void reserve_page(struct hstate *h, int nid, pid_t pid, struct slot_list *s, int contention_idx, unsigned long addr)
{
   size_t bucket = pfn_to_bucket(page_to_pfn(s->page));

   spin_lock(&lock);
   s->used_by = pid;
   s->heat = 0;
   s->virt_addr = addr;
   list_move(&s->contention_list, &allocated_pages[contention_idx]);
   spin_unlock(&lock);

   atomic_add(50, &heatmap[bucket]);
   move_pages_contention(bucket, atomic_read(&heatmap[bucket]));

   list_move(&s->page->lru, &h->hugepage_activelist);
   set_page_count(s->page, 1);
   ClearHPageFreed(s->page);
   h->free_huge_pages--;
   h->free_huge_pages_node[nid]--;

   total_allocated_pages++;
   //printk("Allocating page %lu heat %d here %lu %lx\n", pfn_to_large_pfn(page_to_pfn(s->page)), atomic_read(&heatmap[bucket]), addr, addr);
}

struct hstate *size_to_hstate(unsigned long size)
{
	struct hstate *h;
   struct hstate *hstates = get_hstates();
   int hugetlb_max_hstate = get_max_hstates();

	for_each_hstate(h) {
		if (huge_page_size(h) == size)
			return h;
	}
	return NULL;
}

/*
 * Function called at initialization time to build the list of 2MB pages
 */
static void build_page_list(void) {
   size_t i, nb_pages = 0;
   int nid = 0; // TODO!
   struct hstate *h;
   struct page *page;
   bool pin = !!(current->flags & PF_MEMALLOC_PIN);
   struct hstate *hstates = get_hstates();
   int hugetlb_max_hstate = get_max_hstates();

   for(i = 0; i < CONTENTION_LEVELS; i++)
      INIT_LIST_HEAD(&unused_pages[i]);
   for(i = 0; i < CONTENTION_LEVELS; i++)
      INIT_LIST_HEAD(&allocated_pages[i]);
   for(i = 0; i < CONTENTION_LEVELS; i++)
      INIT_LIST_HEAD(&heated_pages[i]);
   for(i = 0; i < CACHE_SIZE; i++)
      INIT_LIST_HEAD(&cache_bins[i]);

   // Pages have 0 refcount here -- but let's not increase it to not interfere with the kernel
   // They are not going to disappear anyway, except if the user reduces the number of
   // hugetlbfs pages...
   for_each_hstate(h) {
      list_for_each_entry(page, &h->hugepage_freelists[nid], lru) {
	 //Mofification By OSM
	 //if (pin && !is_pinnable_page(page))
         if (pin && !is_longterm_pinnable_page(page))
            continue;

         if (PageHWPoison(page))
            continue;

         nb_pages++;
         if(add_new_page(page) != 0)
            break;
      }
   }
   printk("Successfully created a list of %lu pages\n", nb_pages);
}

static __attribute__((unused)) void check_list(void) {
   size_t i;
   struct slot_list *s;
   for(i = 0; i < CONTENTION_LEVELS; i++) {
      int nb_entries = 0;
      list_for_each_entry(s, &unused_pages[i], contention_list) {
         nb_entries++;
      }
      printk("CHECK: contention[%lu] = %d entries\n", i, nb_entries);
   }
   return;
   for(i = 0; i < CACHE_SIZE; i++) {
      int nb_entries = 0, nb_allocated = 0;
      list_for_each_entry(s, &cache_bins[i], bin_list) {
         nb_entries++;
         if(s->used_by)
            nb_allocated++;
      }
      printk("CHECK: cache_bins[%lu] = %d entries %d allocated %d heat\n", i, nb_entries, nb_allocated, atomic_read(&heatmap[i]));
   }
}

//Modification By OSM
//static struct page *minimize_conflicts(struct hstate *h, int nid, struct vm_area_struct *vma, unsigned long addr) {
static struct page *minimize_conflicts(struct hstate *h, int nid) {
   //Modification By OSM
   unsigned long addr = 0;
   
   //Modification By OSM
   //return _minimize_conflicts(h, nid, current->tgid, vma, addr);
   return _minimize_conflicts(h, nid, current->tgid, NULL, addr);
}

static struct page *enqueue_freed_page(struct hstate *h, struct page *page) {
   int nid = page_to_nid(page);

	lockdep_assert_held(&hugetlb_lock);
	VM_BUG_ON_PAGE(page_count(page), page);

	list_move(&page->lru, &h->hugepage_freelists[nid]);
	h->free_huge_pages++;
	h->free_huge_pages_node[nid]++;
	SetHPageFreed(page);

   add_freed_page(page);
   return page;
}

/*
 * Init and cleanup
 */
int init_module(void)
{
   size_t config, cpu, ncpus = num_online_cpus();

   /* Clear structures */
   printbuf = vmalloc(HEAT_PRINT_SIZE);
   memset(heatmap, 0, sizeof(heatmap));
   memset(&ring, 0, sizeof(ring));

   /* Create all the metadata for the hugepages */
   build_page_list();

   /* Tell the kernel to use our dequeue/enqueue functions for smart alloc/freeing of huge pages */
   set_dequeue_hook(minimize_conflicts);
   set_enqueue_hook(enqueue_freed_page);

   return 0;
}

void cleanup_module(void)
{
   size_t cpu, config, ncpus = num_online_cpus();

   /* Tell the kernel to stop using our dequeue/enqueue functions */
   set_dequeue_hook(NULL);
   set_enqueue_hook(NULL);

   /* Print some debug info */
   printk(KERN_INFO "Goodbye world. Total allocated pages %d\n", total_allocated_pages);
   
   check_list();
   vfree(printbuf);
}

MODULE_LICENSE("GPL");
