SOURCES = blinky.asm

%.o: %.asm
	ca65 -o $@ -l $(@:.o=.lst) $<

all: blinky

blinky: $(SOURCES:.asm=.o)
	cl65 -C programs.cfg -t none -o $@ $^

clean:
	rm -f blinky *.o *.lst