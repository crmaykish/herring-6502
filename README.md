# Herring 6502

8-bit computer based on the W65C02 CPU and a mix of modern hardware

![Herring 6502 Logo](/assets/images/logo_black_256.png)

## Boards

### Backplane

### CPU Board
TODO

### Memory
TODO

### Parallel I/O
TODO

### Serial I/O
TODO

### Debug Board

TODO

## Memory Map
TODO

## Main Bus Pins

The motherboard and daughterboards are connected with a 40 pin single-row connector. Most of these pins come directly from the 65C02. They are mapped in the following order:

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

TODO: 35 - 40

## References

### Inspiration

1. HomeComputer 6502: https://www.grappendorf.net/projects/6502-home-computer/

### Guides

1. How to read SD cards with a 6502: https://github.com/gfoot/sdcard6502
2. ACIA 6551: http://retro.hansotten.nl/6502-sbc/lee-davison-web-site/acia-6551/