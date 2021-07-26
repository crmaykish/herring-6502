#!/bin/sh

TARGET=${1} # folder name
TYPE=${2} # loader or program

echo "Building ${1}"

mkdir -p ${TARGET}/build

cl65 -O \
    -t none \
    --cpu 65c02 \
    -I hardware/include/ \
    --asm-include-dir hardware/include \
    -I ${TARGET}/include/ \
    --asm-include-dir ${TARGET}/include \
    -C config/${TYPE}.cfg \
    -o ${TARGET}/build/firmware.bin \
    -m ${TARGET}/build/loader.map \
    ${TARGET}/src/* hardware/src/* \
    config/${TYPE}.lib

mv ${TARGET}/src/*.o ${TARGET}/build/
mv hardware/src/*.o ${TARGET}/build/

echo "Done!"
