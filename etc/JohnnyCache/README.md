//Need Kenel modification

vim mm/hugetlb.c
int isolate_hugetlb()
{
  // ...
}

# Modification By OSM
EXPORT_SYMBOL(isolate_hugetlb)
