#!/bin/bash

cd /homs/osm/DCP/gperftoll

# Set TCMMALLOC
sudo ./autogen.sh
sudo ./configure
sudo make -j26 & make install -j26
