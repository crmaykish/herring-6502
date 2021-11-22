#!/bin/sh
cd /home/colin/Workspace/herring-6502/tools/memulator
rm -rf build
mkdir build
cd build
cmake ../ -DPICO_SDK_PATH=/mnt/pico/pico/pico-sdk
make -j4
cd ..