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
SRCS = $(shell awk '/^add_file/ && $$2 ~ /\.v$$/ {print $$2}' build.tcl)
INPUTS = build.tcl $(TOP).cst $(TOP).sdc $(ROM_HEX) $(SRCS)

$(BIT): $(INPUTS)
	$(GW) build.tcl

# Always rebuild the monitor firmware so the ROM image tracks the latest source
monitor-rom:
	$(MAKE) -C ../firmware monitor

# Convert the raw monitor binary into one-hex-byte-per-line for $readmemh
$(ROM_HEX): monitor-rom
	@hexdump -v -e '1/1 "%02x\n"' $(ROM_BIN) > $(ROM_HEX).tmp
	@if cmp -s $(ROM_HEX).tmp $(ROM_HEX); then \
		rm -f $(ROM_HEX).tmp; \
		echo "$(ROM_HEX): up to date"; \
	else \
		mv $(ROM_HEX).tmp $(ROM_HEX); \
		echo "$(ROM_HEX): regenerated from $(ROM_BIN)"; \
	fi

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
	rm -f $(ROM_HEX) $(ROM_HEX).tmp *.vvp *.vcd

.PHONY: cores monitor-rom prog flash detect clean
