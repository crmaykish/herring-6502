# Herring 6502
8-bit computer based on the W65C02 CPU and a mix of modern hardware

## Boards

1. Motherboard: Power and main bus fed to all daughterboards (passive circuitry)
2. CPU Board: W65C02 CPU and crystal oscillator, switch between single step and free-run modes
3. ROM Board: Holds a single 32kb EEPROM chip
4. RAM Board: Holds a single 32kb SRAM chip
5. Display Board: Memory-mapped control of a large RGB LED matrix acting as a display
6. Input Board: ATmega32U4 translating USB keyboard signals to input for the W65C02 databus
7. Debug Board: ATmega644p MCU watching the address bus, data bus, and control signals. Controls the clock via serial commands

## Memory Map

0x0000 - 0x7FFF : RAM (32k)
0x8000 - 0xBFFF : I/O
0xC000 - 0xFFFF : ROM (16k)

## Main Bus Pins

The motherboard and daughterboards are connected with a 36 pin single-row connector with the following pinout:

1. VCC
2. GND

3. A15
4. A14
5. A13
6. A12
7. A11
8. A10
9. A9
10. A8
11. A7
12. A6
13. A5
14. A4
15. A3
16. A2
17. A1
18. A0

19. D7
20. D6
21. D5
22. D4
23. D3
24. D2
25. D1
26. D0

27. RESB
28. RDY
29. IRQB
30. PHI2
31. MLB
32. NMIB
33. SYNC
34. RWB
35. N/C
36. N/C


## Notes

1. Use pin 35 and 36 as address decoder pins? Are two pins enough? RAM, ROM, Video, Input? Additional boards could have their own address decoding if necessary
