C_SOURCES = main.c
ASM_SOURCES = startup.s

%.o: %.c
	cc65 --cpu 6502 -O -t none -o $(@:.o=.s) $<
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $(<:.c=.s)

%.o: %.s65
	ca65 --cpu 6502 -o $@ -l $(@:.o=.lst) $<

firmware: $(ASM_SOURCES:.s65=.o) $(C_SOURCES:.c=.o)
	cl65 -C firmware.cfg -m firmware.map -o $@ $^ cc65.lib
