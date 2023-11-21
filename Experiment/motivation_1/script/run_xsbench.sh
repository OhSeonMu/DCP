#!/bin/bash

XSBench_path="/home/osm/DCP/XSBench/openmp-threading"
output_path="/home/osm/DCP/Experiment/motivation_1/output"

for size in "small" "large" "XL" "XXL"
do
    $XSBench_path/XSBench -s $size >> $output_path/$1_$size
done
