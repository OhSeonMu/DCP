#!/bin/bash

# Reserve a large pages
sudo sh -c "echo 501760 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages"
#sudo sh -c "echo 6072 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages" # 600GB
sudo mkdir /mnt/hugetlbfs
sudo mount -t hugetlbfs none /mnt/hugetlbfs
sudo chmod 777 /mnt/hugetlbfs/

