
#!/bin/bash

for benchmark in "xsbench" "gapbs"
do
    ./run_$benchmark.sh $1
done
