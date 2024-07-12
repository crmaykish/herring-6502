# Herring 6502

Herring is an 8-bit computer built around the 65C02 CPU. It is designed as a backplane system. The backplane board is passive, providing only power, a reset switch, and system bus headers.
The system bus is exposed as single-row 40-pin female headers using standard 0.1 inch pin pitch.

![Herring 6502 v1.5](/assets/images/herring_v1.5.jpg)

## Hardware

The memory map is set up in the following way:

1. 0x0000 - 0x7FFF: Low RAM (32K)
2. 0x8000 - 0xBFFF: Bankable RAM (16K pages)
3. 0xC000 - 0xDFFF: Memory-mapped I/O (8K)
4. 0xE000 - 0xFFFF: ROM (8K)

Individual component boards are connected to the backplane via the 40-pin system bus. Most of the pins on the bus come directly from the 65C02 plus a few for peripheral support. They are mapped from left to right in the following order:

1. VCC (5v regulated)
2. GND
3. D0
4. D1
5. D2
6. D3
7. D4
8. D5
9. D6
10. D7
11. A0
12. A1
13. A2
14. A3
15. A4
16. A5
17. A6
18. A7
19. A8
20. A9
21. A10
22. A11
23. A12
24. A13
25. A14
26. A15
27. RESB - Reset (Active low)
28. CLK - System clock
29. RWB - Read/write flag (Read = high, Write = low)
30. IRQB - Interrupt request (Active low)
31. DEC0 - Functions as the RAM write-enable pin
32. DEC1 - Peripheral enable
33. DEC2 - Peripheral enable
34. DEC3 - Peripheral enable
35. DEC4 - Peripheral enable
36. DEC5 - Peripheral enable
37. DEC6 - Peripheral enable
38. DEC7 - Peripheral enable
39. BE - Bus Enable (Active high)
40. GND

## Software

Herring currently runs a simple monitor program from ROM that provides the basic peek, poke, jump commands as well as memory clearing and memory dumping. This monitor also functions as a bootloader, allowing new firmware to be copied into RAM from a host PC via serial port. I use a simple program on the PC to transfer firmware: https://github.com/crmaykish/ctt in this way.

All firmware development is done in C and uses the cc65 set of tools.

## Further Reading

See https://hackaday.io/project/181802-herring-6502-computer for project logs and more details on the hardware.