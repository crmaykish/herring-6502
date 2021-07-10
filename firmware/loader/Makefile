SOURCES = loader.asm

%.o: %.asm
	ca65 -o $@ -l $(@:.o=.lst) $<

all: loader

loader: $(SOURCES:.asm=.o)
	cl65 -C loader.cfg -t none -o $@ $^

clean:
	rm -f loader *.o *.lst