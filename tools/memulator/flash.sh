#!/bin/sh
sudo mount /dev/sda1 /mnt/pico
sudo cp build/memulator.uf2 /mnt/pico
sync
sudo umount /mnt/pico