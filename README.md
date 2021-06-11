# herring-6502
8-bit computer based on the W65C02 CPU and a mix of modern hardware

## Design Notes

### Boards

1. Motherboard: Power and main bus
2. CPU and clock: switch between free-run and single-step mode
3. ROM (32k): $8000-$FFFF
4. RAM (32k): $0000-$7FFF
5. Video: FPGA controlling LED panel
6. Debug Board: Output Address and Data lines to LCD display and LEDs

Note: address mapping is going to be a pain

### Bus Pins

From left to right

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
