C_SOURCES = loader.c
ASM_SOURCES = zeropage.asm crt0.asm run.asm

%.o: %.c
	cc65 --include-dir ../include --cpu 6502 -O -t none -o $(@:.o=.s) $<
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $(<:.c=.s)

%.o: %.asm
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $<

all: loader.bin

loader.bin: $(ASM_SOURCES:.asm=.o) $(C_SOURCES:.c=.o)
	cl65 -C loader.cfg -m herring.map -o $@ $^ loader.lib

clean:
	rm -f *.bin *.s *.o *.lst *.map

