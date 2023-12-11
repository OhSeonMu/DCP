#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x92997ed8, "_printk" },
	{ 0xd5cdceb2, "current_task" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xd1d6e0b7, "boot_cpu_data" },
	{ 0xa94a09bb, "mem_section" },
	{ 0xf031d163, "get_hstates" },
	{ 0x88949a76, "get_max_hstates" },
	{ 0xa648e561, "__ubsan_handle_shift_out_of_bounds" },
	{ 0xbcb36fe4, "hugetlb_optimize_vmemmap_key" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x6bb82ab0, "kmalloc_caches" },
	{ 0x6dc3bac2, "kmalloc_trace" },
	{ 0xc8dcc62a, "krealloc" },
	{ 0xc2f7f2c2, "task_active_pid_ns" },
	{ 0x6ecc32a0, "find_pid_ns" },
	{ 0xd6b0b1c3, "pid_task" },
	{ 0x8d522714, "__rcu_read_lock" },
	{ 0x2469810f, "__rcu_read_unlock" },
	{ 0x90d5fec, "get_task_mm" },
	{ 0x759492aa, "__tracepoint_mmap_lock_start_locking" },
	{ 0x668b19a1, "down_read" },
	{ 0x66ad1fcf, "__tracepoint_mmap_lock_acquire_returned" },
	{ 0xbbd2de83, "find_vma" },
	{ 0x7c0275f0, "__tracepoint_mmap_lock_released" },
	{ 0x53b954a2, "up_read" },
	{ 0x8ea3d5ef, "migrate_pages" },
	{ 0x288d825b, "mmput" },
	{ 0x14817227, "__mmap_lock_do_trace_acquire_returned" },
	{ 0xd79ebc4a, "__mmap_lock_do_trace_start_locking" },
	{ 0x4b17f8c8, "__mmap_lock_do_trace_released" },
	{ 0x93996ca5, "__put_task_struct" },
	{ 0x296695f, "refcount_warn_saturate" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xc60d0620, "__num_online_cpus" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0xfb578fc5, "memset" },
	{ 0x85efc7e0, "zero_pfn" },
	{ 0x6872c04d, "set_dequeue_hook" },
	{ 0x417e9483, "set_enqueue_hook" },
	{ 0xecace7dc, "kthread_create_on_node" },
	{ 0x5667e975, "wake_up_process" },
	{ 0x52979791, "kthread_stop" },
	{ 0x999e8297, "vfree" },
	{ 0x59fd2c4f, "param_ops_int" },
	{ 0x6a2d88a9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "CB86F8CAB0B49D150BD192A");
