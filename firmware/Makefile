C_SOURCES = monitor.c
ASM_SOURCES = zeropage.asm startup.asm

%.o: %.c
	cc65 --cpu 6502 -O -t none -o $(@:.o=.s) $<
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $(<:.c=.s)

%.o: %.s65
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $<

all: monitor.bin

monitor.bin: $(ASM_SOURCES:.s65=.o) $(C_SOURCES:.c=.o)
	cl65 -C herring.cfg -m herring.map -o $@ $^

clean:
	rm -f monitor.bin *.s *.o *.lst *.map