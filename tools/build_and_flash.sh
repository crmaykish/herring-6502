#!/bin/sh

set -e

SERIAL_PORT="/dev/ttyACM0"

echo "### ASSEMBLER ###"
~/vasm6502_oldstyle -Fbin -dotdir $1

BIN_SIZE=$(wc -c < a.out)
ROM_START=49152 # 0xC000

echo "### HEX DUMP ####"
hexdump -C a.out

echo "### FLASHING ROM ###"
python3 /home/colin/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d $SERIAL_PORT -w -l $BIN_SIZE -f a.out -o $ROM_START

echo "### READ FLASH ###"
python3 /home/colin/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d $SERIAL_PORT -r -l $BIN_SIZE -o $ROM_START

rm a.out

echo "### DONE ###"

set +e
