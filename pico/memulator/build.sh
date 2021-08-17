#!/bin/sh
rm -rf build
mkdir build
cd build
cmake ../ -DPICO_SDK_PATH=/home/colin/Workspace/pico/pico-sdk
make -j4
cd ..