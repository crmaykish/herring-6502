#!/bin/sh

echo "Assembling program..."
vasm6502_oldstyle -Fbin -dotdir asm/ram_test.s

echo "Programming EEPROM..."

python3 ~/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d /dev/ttyACM0 -w -l 30 -f a.out

echo "Programming finished."

python3 ~/Workspace/AT28C-EEPROM-Programmer-Arduino/at28c_programmer.py -d /dev/ttyACM0 -r -l 30

