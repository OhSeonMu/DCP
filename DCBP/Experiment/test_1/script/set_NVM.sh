#!/bin/bash

sudo ndctl create-namespace -M dev -m devdax -s 512G
sudo daxctl reconfigure-device dax0.0 -N -m system-ram
