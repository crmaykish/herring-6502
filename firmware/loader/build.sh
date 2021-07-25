#!/bin/sh

echo "Building loader"

mkdir -p build

cl65 -t none --cpu 65c02 -O -I include/ --asm-include-dir include -C config/loader.cfg -o build/loader.bin -m build/loader.map src/* config/loader.lib

mv src/*.o build

echo "Done!"