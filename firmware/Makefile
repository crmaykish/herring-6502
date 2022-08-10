CC = cl65
CA = ca65
AR = ar65

# TODO: the optimization flags are having weird effects on the USB code
# Might be the way I'm doing register access. PEEK/POKE might be better
CFLAGS = -O -t none --cpu 65c02 --static-locals -I include

ROM_CFG = config/rom.cfg
RAM_CFG = config/ram.cfg

ROM_LIB = config/rom.lib
RAM_LIB = config/ram.lib

COMMON_SRC = src/herring.c src/delay.c

MONITOR_SRC = $(COMMON_SRC) src/monitor.c src/serial.c
HELLO_SRC = $(COMMON_SRC) src/hello.c src/serial.c
CONWAY_SRC = $(COMMON_SRC) src/conway.c src/serial.c
CHIP8_SRC = $(COMMON_SRC) src/chip8_emulator.c src/serial.c libs/chip8/src/chip8.c

.PHONY: all clean

all: monitor hello conway chip8

# Firmware applications

# Herring Monitor/Bootloader
monitor: $(MONITOR_SRC) | build_dir rom_lib
	$(CC) $(CFLAGS) -C $(ROM_CFG) $(MONITOR_SRC) $(ROM_LIB) -o build/monitor.bin

# Hello World
hello: $(HELLO_SRC) | build_dir ram_lib
	$(CC) $(CFLAGS) -C $(RAM_CFG) $(HELLO_SRC) $(RAM_LIB) -o build/hello.bin

# Game of life
conway: $(CONWAY_SRC) | build_dir ram_lib
	$(CC) $(CFLAGS) -C $(RAM_CFG) $(CONWAY_SRC) $(RAM_LIB) -o build/conway.bin

# CHIP-8 Emulator
chip8: $(CHIP8_SRC) | build_dir ram_lib
	$(CC) $(CFLAGS) -I libs/chip8/include -C $(RAM_CFG) $(CHIP8_SRC) $(RAM_LIB) -o build/chip8.bin

# Standard libraries

# TODO: don't really need to build the libs every time we build firmware
# Only rebuild when the crt0 files change

rom_lib:
	cp /usr/share/cc65/lib/supervision.lib config/rom.lib
	$(CA) -t none --cpu 65c02 -o config/crt0.o config/crt0_rom.asm 
	$(AR) a config/rom.lib config/crt0.o

ram_lib:
	cp /usr/share/cc65/lib/supervision.lib config/ram.lib
	$(CA) -t none --cpu 65c02 -o config/crt0.o config/crt0_ram.asm 
	$(AR) a config/ram.lib config/crt0.o

# Cleanup

build_dir:
	@mkdir -p build

clean:
	@$(RM) -rf build src/*.o config/*.lib config/*.o build/*.map build/*.list
