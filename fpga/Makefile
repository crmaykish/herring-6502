export QT_QPA_PLATFORM := offscreen

TOP      = herring_f
BIT      = impl/pnr/$(TOP).fs

GW       = LD_LIBRARY_PATH=$(HOME)/Gowin_EDA/IDE/lib $(HOME)/Gowin_EDA/IDE/bin/gw_sh
LOADER   = $(HOME)/oss-cad-suite/bin/openFPGALoader
IVERILOG = $(HOME)/oss-cad-suite/bin/iverilog
VVP      = $(HOME)/oss-cad-suite/bin/vvp

ROM_BIN  = ../firmware/build/monitor.bin
ROM_HEX  = rom.hex

# Re-synthesize when any source, constraint, or the ROM image changes
SRCS = $(wildcard rtl/*.v) $(wildcard cores/verilog-6502/*.v) $(wildcard cores/verilog-65c51/rtl/*.v)
INPUTS = build.tcl $(TOP).cst $(TOP).sdc $(ROM_HEX) $(SRCS)

$(BIT): $(INPUTS)
	$(GW) build.tcl

# Convert the raw monitor binary into one-hex-byte-per-line for $readmemh
$(ROM_HEX): $(ROM_BIN)
	hexdump -v -e '1/1 "%02x\n"' $(ROM_BIN) > $(ROM_HEX)

# Fetch third-party cores into cores/
cores:
	./get_cores.sh

# Load the bitstream into SRAM (volatile)
prog: $(BIT)
	$(LOADER) -b tangnano9k $(BIT)

# Load the bitstream into SPI flash (persistent)
flash: $(BIT)
	$(LOADER) -b tangnano9k -f $(BIT)

clean:
	rm -rf impl
	rm -f $(ROM_HEX) *.vvp *.vcd

.PHONY: cores prog flash detect clean
