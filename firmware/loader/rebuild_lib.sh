#!/bin/sh

echo "Removing existing lib file"
rm -f config/*.lib

echo "Copy supervision.lib"
cp /usr/share/cc65/lib/supervision.lib config/loader.lib

echo "Assemble crt0.asm"
ca65 --cpu 65c02 src/crt0.asm

echo "Add crt0.asm to lib file"
ar65 a config/loader.lib src/crt0.o

rm src/crt0.o
