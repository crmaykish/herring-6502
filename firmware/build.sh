#!/bin/sh

TARGET=${1} # program name
TYPE=${2} # 'ram' or 'rom'

echo "Building ${1}"

mkdir -p build

cl65 -O \
    -t none \
    --cpu 65c02 \
    --static-locals \
    -I include/ \
    --asm-include-dir include \
    -C config/${TYPE}.cfg \
    -o build/${TARGET}.bin \
    src/libs/* src/${TARGET}.c \
    config/${TYPE}.lib

mv src/*.o build
mv src/libs/*.o build

echo "Done!"
