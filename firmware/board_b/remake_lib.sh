#!/bin/sh

rm *.lib

cp /usr/share/cc65/lib/supervision.lib board_b.lib

ca65 --cpu 65c02 crt0.asm

ar65 a board_b.lib crt0.o
