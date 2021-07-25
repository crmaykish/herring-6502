#!/bin/sh

echo "Building board B binary..."

cc65 -t none -O --cpu 65c02 -I ../include main.c
cc65 -t none -O --cpu 65c02 -I ../include ../acia.c

ca65 --cpu 65c02 main.s
ca65 --cpu 65c02 ../acia.s

ld65 -C board_b.cfg -m main.map main.o ../acia.o -o main.bin board_b.lib

echo "Done"