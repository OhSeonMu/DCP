#!/bin/bash

XSBench_path="/home/osm/DCP/app/XSBench/openmp-threading"
output_path="/home/osm/DCP/Experiment/motivation_1/output"

for size in "small" "large" "XL" "XXL"
do
    $XSBench_path/XSBench -s $size -p 10000000 > $output_path/XSBench_$1_$size
done
