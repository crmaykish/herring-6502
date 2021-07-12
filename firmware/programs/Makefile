SOURCES = blinky.asm

%.o: %.asm
	ca65 -o $@ -l $(@:.o=.lst) $<

all: blinky.bin serial_demo.bin

blinky.bin: $(SOURCES:.asm=.o)
	cl65 -C programs.cfg -t none -o $@ $^

serial_demo.bin: serial_demo.asm
	cl65 -C programs.cfg -t none -o $@ $^

clean:
	rm -f *.o *.lst *.bin