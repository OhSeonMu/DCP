#!/bin/bash

XSBench_path="/home/osm/DCP/app/XSBench/openmp-threading"
gups_path="/home/osm/DCP/app/gups"
output_path="/home/osm/DCBP/Experiment/test_1/output"

for size in "large" "XL" "XXL"
do
    # For App-direct or Mixed Moed
    if [ $1 == "App_Direct" -o $1 == "Mixed_Mode_App_Direct" ]; then
        numactl --cpunodebind=0 --membind=1 $XSBench_path/XSBench -s $size > $output_path/$1_XSBench_$size
    # For Memory Mode
    elif [ $1 == "Memory_Mode" -o $1 == "Mixed_Mode_Memory_Mode"  ]; then
        numactl --cpunodebind=0 --membind=0 $XSBench_path/XSBench -s $size > $output_path/$1_XSBench_$size
    fi
done

: << "END"
for size in  
do
    # For App-direct or Mixed Moed
    if [ $1 == "App_Direct" -o $1 == "Mixed_Mode_App_Direct" ]; then
        numactl --cpunodebind=0 --membind=1 taskset -c 0 $gups_path/gups_vanilla $size 100000 1024 > $output_path/$1_gupbs_$size
    # For Memory Mode
    elif [ $1 == "Memory_Mode" -o $1 == "Mixed_Mode_Memory_Mode"  ]; then
        numactl --cpunodebind=0 --membind=1 taskset -c 0 $gups_path/gups_vanilla $size 10000000 1024 > $output_path/$1_gupbs_$size
    fi
done
END
