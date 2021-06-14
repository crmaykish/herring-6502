# Herring 6502

8-bit computer based on the W65C02 CPU and a mix of modern hardware

![Herring 6502 Logo](/assets/images/logo_black_256.png)

## Boards

1. Motherboard: Power and main bus fed to all daughterboards (passive circuitry)
2. CPU Board: W65C02 CPU and crystal oscillator, switch between single step and free-run modes
3. ROM Board: Holds a single 32kb EEPROM chip
4. RAM Board: Holds a single 32kb SRAM chip
5. Display Board: Memory-mapped control of a large RGB LED matrix acting as a display
6. I/O Board: W65C22 VIA chip exposing two 8-bit GPIO ports
7. Debug Board: ATmega644p MCU watching the address bus, data bus, and control signals. Controls the clock via serial commands

## Memory Map

| Device | Start | End | Bytes | A15 | A14 | A13 | 35 | 36 |
| ------ | ----- | --- | ----- | --- | --- | --- | -- | -- |
| RAM | 0x0000 | 0x7FFF | 0x8000 | 0 | X | X | 0 | 0 |
| VIA | 0x8000 | 0x9FFF | 0x2000 | 1 | 0 | 0 | 0 | 1 |
| Video | 0xA000 | 0xBFFF | 0x2000 | 1 | 0 | 1 | 1 | 0 |
| ROM | 0xC000 | 0xFFFF | 0x4000 | 1 | 1 | X | 1 | 1 |

## Main Bus Pins

The motherboard and daughterboards are connected with a 36 pin single-row connector. Most of these pins come directly from the 65C02. They are mapped in the following order:

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
35. Y: Decoder output bit 1
36. Z: Decoder output bit 2

## Notes

1. Add an AND gate to the RAM R/W pin and CLK to only write on the clock pulse (seems to be working without it at low speed, but may cause errors at full speed)
