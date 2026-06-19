CC = cl65
CA = ca65
AR = ar65

# Locate the cc65 lib dir from cl65
CC65_LIB_DIR := $(shell dirname $$(cl65 --print-target-path))/lib

CFLAGS = -O -t none --cpu 65c02 --static-locals -I include

ROM_CFG = config/rom.cfg
RAM_CFG = config/ram.cfg

ROM_LIB = config/rom.lib
RAM_LIB = config/ram.lib

COMMON_SRC = src/herring.c

MONITOR_SRC = $(COMMON_SRC) src/monitor.c src/serial.c
CONWAY_SRC = $(COMMON_SRC) src/conway.c src/serial.c

.PHONY: all clean

all: monitor conway

# Firmware applications

# Herring Monitor/Bootloader
monitor: $(MONITOR_SRC) | build_dir rom_lib
	$(CC) $(CFLAGS) -C $(ROM_CFG) $(MONITOR_SRC) $(ROM_LIB) -o build/monitor.bin

# Game of life
conway: $(CONWAY_SRC) | build_dir ram_lib
	$(CC) $(CFLAGS) -C $(RAM_CFG) $(CONWAY_SRC) $(RAM_LIB) -o build/conway.bin

# Standard library (ROM)
rom_lib:
	cp $(CC65_LIB_DIR)/supervision.lib config/rom.lib
	$(CA) -t none --cpu 65c02 -o config/crt0.o config/crt0_rom.asm
	$(AR) a config/rom.lib config/crt0.o

# Standard library (RAM)
ram_lib:
	cp $(CC65_LIB_DIR)/supervision.lib config/ram.lib
	$(CA) -t none --cpu 65c02 -o config/crt0.o config/crt0_ram.asm
	$(AR) a config/ram.lib config/crt0.o

# Cleanup

build_dir:
	@mkdir -p build

clean:
	@$(RM) -rf build src/*.o config/*.lib config/*.o build/*.map build/*.list
