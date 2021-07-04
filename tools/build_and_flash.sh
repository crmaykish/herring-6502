#!/bin/sh

set -e

SERIAL_PORT="/dev/ttyACM0"

echo "### ASSEMBLING ###"
ca65 --cpu 6502 -t none -o ${1}.o ${1}
ld65 -C firmware/herring.cfg ${1}.o -m fw.map

BIN_SIZE=$(grep -m 1 "CODE" fw.map | grep -Po 'Size=\K[^"]*' | cut -d " " -f 1)
ROM_START=49152 # 0xC000

echo "### HEX DUMP ####"
hexdump -C a.out

echo "### FLASHING ROM ###"
python3 /home/colin/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d $SERIAL_PORT -w -l $BIN_SIZE -f a.out -o $ROM_START

echo "### READ FLASH ###"
python3 /home/colin/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d $SERIAL_PORT -r -l $BIN_SIZE -o $ROM_START

echo "### CLEANUP ###"
rm a.out
rm firmware/*.o
rm fw.map

echo "### DONE ###"

set +e
