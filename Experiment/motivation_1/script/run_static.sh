#!/bin/bash

MODE="Static"
LINE_NUMBER=124

DRAM_CACHE_SIZE=48 # 12 24 48 96
JC_MODULE_PATH=/home/osm/DCP/JohnnyCache
TCMALLOC_PATH=/mnt/hugetlbfs/
PRELOAD_PATH=/home/osm/DCP/gperftools/.libs/libtcmalloc.so

XSBench_path="/home/osm/DCP/app/XSBench/openmp-threading"
XSBench_output_path="/home/osm/DCP/Experiment/motivation_1/output"

Gapbs_path="/home/osm/DCP/app/gapbs"  
Gapbs_output_path="/home/osm/DCP/Experiment/motivation_1/output"

/home/osm/DCP/Experiment/script/Johnny_Cache_set.sh

# Set JC module
NEW_LINE="#define CACHE_SIZE ($DRAM_CACHE_SIZE*512)"
sed -i "${LINE_NUMBER}s/.*/$NEW_LINE/" "$JC_MODULE_PATH/jc.c"
cd $JC_MODULE_PATH
make
sudo insmod $JC_MODULE_PATH/jc.ko

for size in "small" "large" "XL" "XXL"
do
   TCMALLOC_MEMFS_MALLOC_PATH=$TCMALLOC_PATH LD_PRELOAD=$PRELOAD_PATH $XSBench_path/XSBench -s $size -p 10000000 > $XSBench_output_path/${MODE}_XSBench_${size}_${DRAM_CACHE_SIZE}
done
	
for size in 25 26 27 28 29
do
   TCMALLOC_MEMFS_MALLOC_PATH=$TCMALLOC_PATH LD_PRELOAD=$PRELOAD_PATH  $Gapbs_path/bc -g $size -n 3 > $Gapbs_output_path/${MODE}_BC_${size}_${DRAM_CACHE_SIZE}
done 

sudo rmmod jc
