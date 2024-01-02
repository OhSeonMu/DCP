#!/bin/bash

Gapbs_path="/home/osm/DCP/app/gapbs"
output_path="/home/osm/DCBP/Experiment/test_1/output"

for size in 24 25 26 27 28
do
   # numactl --cpunodebind=0 --membind=1 $Gapbs_path/bc -g $size -n 3 > $output_path/BC_Mixed_Mode_App_Direct_$size
   # numactl --cpunodebind=0 --membind=0 $Gapbs_path/bc -g $size -n 3 > $output_path/BC_Mixed_Mode_Memory_Mode_$size
   # For App-direct or Mixed Moed
   if [ $1 == "App_Direct" -o $1 == "Mixed_Mode_App_Direct" ]; then
      numactl --cpunodebind=0 --membind=1 $Gapbs_path/bc -g $size -n 3 > $output_path/BC_$1_$size
   # For Memory Mode
   elif [ $1 == "Memory_Mode" -o $1 == "Mixed_Mode_Memory_Mode"  ]; then
      numactl --cpunodebind=0 --membind=0 $Gapbs_path/bc -g $size -n 3 > $output_path/BC_$1_$size
   fi
done
