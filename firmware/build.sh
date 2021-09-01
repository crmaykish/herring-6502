#!/bin/sh

TARGET=${1} # folder name
TYPE=${2} # loader or program

echo "Building ${1}"

mkdir -p ${TARGET}/build

cl65 -O \
    -t none \
    --cpu 65c02 \
    --static-locals \
    -I libs/hardware/include/ \
    --asm-include-dir libs/hardware/include \
    -I ${TARGET}/include/ \
    --asm-include-dir ${TARGET}/include \
    -C config/${TYPE}.cfg \
    -o ${TARGET}/build/firmware.bin \
    -m ${TARGET}/build/${TARGET}.map \
    ${TARGET}/src/* libs/hardware/src/* \
    config/${TYPE}.lib

mv ${TARGET}/src/*.o ${TARGET}/build/
mv libs/hardware/src/*.o ${TARGET}/build/

echo "Done!"
