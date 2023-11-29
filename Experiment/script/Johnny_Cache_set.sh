#!/bin/bash

cd /home/osm/DCP/JohnnyCache

# Reserve a large pages
sudo sh -c "echo 174080 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages"
sudo mkdir /mnt/hugetlbfs
sudo mount -t hugetlbfs none /mnt/hugetlbfs
sudo chmod 777 /mnt/hugetlbfs/

# Insert Module
make
sudo insmod jc.ko