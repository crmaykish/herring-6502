ASM_SOURCES = zeropage.asm crt0.asm
SYSMON_ASM_SOURCES = zeropage.asm crt0.asm ../spi.asm

COUNTER_SOURCES = counter.c
PRIMES_SOURCES = primes.c ../acia.c
SYSMON_SOURCES = sysmon.c ../acia.c ../utils.c ../spi_impl.c
MATRIX_SOURCES = matrix.c ../acia.c ../utils.c

%.o: %.c
	cc65 --include-dir ../include --cpu 6502 -O -t none -o $(@:.o=.s) $<
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $(<:.c=.s)

%.o: %.asm
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $<

all: blinky.bin serial_demo.bin counter.bin sysmon.bin primes.bin matrix.bin

blinky.bin: blinky.asm
	cl65 -C programs.cfg -t none -o $@ $^

serial_demo.bin: serial_demo.asm
	cl65 -C programs.cfg -t none -o $@ $^

counter.bin: $(ASM_SOURCES:.asm=.o) $(COUNTER_SOURCES:.c=.o)
	cl65 --include-dir ../include -C programs.cfg -o $@ $^ programs.lib

sysmon.bin: $(SYSMON_ASM_SOURCES:.asm=.o) $(SYSMON_SOURCES:.c=.o)
	cl65 --include-dir ../include -C programs.cfg -o $@ $^ programs.lib

primes.bin: $(ASM_SOURCES:.asm=.o) $(PRIMES_SOURCES:.c=.o)
	cl65 --include-dir ../include -C programs.cfg -o $@ $^ programs.lib

matrix.bin: $(ASM_SOURCES:.asm=.o) $(MATRIX_SOURCES:.c=.o)
	cl65 --include-dir ../include -C programs.cfg -o $@ $^ programs.lib

clean:
	rm -f *.bin *.s *.o *.lst *.map
	cd ../; rm -f *.bin *.s *.o *.lst *.map