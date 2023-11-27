#!/bin/bash

Gapbs_path="/home/osm/DCP/app/gapbs"
output_path="/home/osm/DCP/Experiment/motivation_1/output"

for size in 25 26 27 28 29
do
    $Gapbs_path/bc -g $size -n 3 > $output_path/BC_$1_$size
done
