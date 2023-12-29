#!/bin/bash

sudo ndctl destroy-namespace namespace0.0 -f
ipmctl delete -goal

if [ $1 == "Memory_Mode" ]; then
	sudo ipmctl create -goal MemoryMode=100
elif [ $1 == "App_Direct" ]; then
	sudo ipmctl create -goal PersistentMemoryType=AppDirect
elif [ $1 == "Mixed_Mode" ]; then
	sudo ipmctl create -goal MemoryMode=50 PersistentMemoryType=AppDirect
fi
