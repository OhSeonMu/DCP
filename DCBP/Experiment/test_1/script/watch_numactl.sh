#!/bin/bash

rm /home/osm/DCBP/Experiment/test_1/output/numactl_result
while true; do
  numactl -H | grep free >> /home/osm/DCBP/Experiment/test_1/output/numactl_result
  sleep 1  # 5분(300초) 간격으로 변경 가능
done

