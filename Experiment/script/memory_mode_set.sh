#!/bin/bash

sudo ipmctl delete -goal
sudo ipmctl create -goal MemoryMode=100
sudo reboot

