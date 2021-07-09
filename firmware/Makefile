C_SOURCES = interpreter.c acia.c via.c basic.c
ASM_SOURCES = zeropage.asm crt0.asm

%.o: %.c
	cc65 --cpu 6502 -O -t none -o $(@:.o=.s) $<
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $(<:.c=.s)

%.o: %.asm
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $<

all: interp.bin

interp.bin: $(ASM_SOURCES:.asm=.o) $(C_SOURCES:.c=.o)
	cl65 -C herring.cfg -m herring.map -o $@ $^ herring.lib

clean:
	rm -f *.bin *.s *.o *.lst *.map

