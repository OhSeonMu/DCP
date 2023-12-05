#!/bin/bash

LINE_NUMBER=126

JC_MODULE_PATH=/home/osm/DCP/code/default
TCMALLOC_PATH=/mnt/hugetlbfs/
PRELOAD_PATH=/home/osm/DCP/gperftools/.libs/libtcmalloc.so

XSBench_path="/home/osm/DCP/app/XSBench/openmp-threading"
XSBench_output_path="/home/osm/DCP/Experiment/motivation_2/output"

Gapbs_path="/home/osm/DCP/app/gapbs"  
Gapbs_output_path="/home/osm/DCP/Experiment/motivation_2/output"

# Set JC module
# for DRAM_CACHE_SIZE in 96 92 88 84 80 76 72 68 64 60 56 52 48 44 40 363 32 28 24 20 16 12 8 4
for DRAM_CACHE_SIZE in 16 32
do
	# Set JC module
	NEW_LINE="#define CACHE_SIZE ($DRAM_CACHE_SIZE*512)"
	sed -i "${LINE_NUMBER}s/.*/$NEW_LINE/" "$JC_MODULE_PATH/default.c"
	cd $JC_MODULE_PATH
	make
	sudo insmod $JC_MODULE_PATH/default.ko
	echo $DRAM_CACHE_SIZE	
	for size in "XL"
	do
		TCMALLOC_MEMFS_MALLOC_PATH=$TCMALLOC_PATH LD_PRELOAD=$PRELOAD_PATH $XSBench_path/XSBench -s $size -p 10000000 > $XSBench_output_path/Default_XSBench_${size}_${DRAM_CACHE_SIZE}
	done
	
	
	#for size in 25 26 27 28 29
	#do
	#	TCMALLOC_MEMFS_MALLOC_PATH=$TCMALLOC_PATH LD_PRELOAD=$PRELOAD_PATH  $Gapbs_path/bc -g $size -n 3 > $Gapbs_output_path/BC_${size}_${DRAM_CACHE_SIZE}
	#done 
	
	sudo rmmod jc
done
