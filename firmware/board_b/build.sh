#!/bin/sh

echo "Building board B binary..."

cc65 -t none -O --cpu 65c02 main.c
ca65 --cpu 65c02 main.s

ld65 -C board_b.cfg -m main.map main.o -o main.bin board_b.lib

echo "Done"