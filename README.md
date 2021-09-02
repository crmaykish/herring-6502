# Herring 6502

8-bit computer based on the W65C02 CPU and a mix of modern hardware

![Herring 6502 Logo](/assets/images/logo_black_256.png)

# Hardware

Herring is designed as a backplane system. The backplane board is passive, providing only power, a reset switch, and system bus headers.
The system bus is exposed as single-row 40-pin female headers using standard 0.1 inch pin pitch.

## Power

Herring can be built as a 3.3v system or as a 5v system depending on the components. Most modern versions of the CPU, memory, and peripheral chips are available with 3.3v support and this voltage makes it easier to interface with modern CPLDs and FPGAs. My main goal for this computer system is to design custom hardware and native 3.3v logic levels allow me to do this without level shifters all over the place. 

## Boards

### CPU
The only truly required board, this board holds a 65C02 (or compatible) CPU and some pullup resistors.

### Memory
A multi-purpose board, this supports DIP-28 SRAM or EEPROM chips in either 8k or 32k sizes. Jumpers are provided to select between sizes and whether the chip is writeable (RAM vs ROM).

### VIA
Breaks out the two GPIO ports of a 65C22 VIA into 2x7 ribbon cables. These ribbon cables can connect into LED boards or adapters for LCD character displays.

### Serial
Uses a 65C51-compatible ACIA chip. Provides the basic serial signals (Tx, Rx, GND) via a pin header. Onboard 1.8432 MHz crystal for baudrate generation.

### Clock / Decoder
There are multiple options for this board. For 3.3v systems, I use a 50 MHz oscillator and a TinyFPGA to handle clock division and address decoding. For a 5v system, the same can be done with a 5v-compatible CPLD or GAL.

## Sourcing Parts

Sourcing parts for a 5v system is generally easier than 3.3v. Almost all components except those that explicitly low voltage variants will work at 5v.

Besides the passive components and connectors/headers, here is a simplified bill of materials:

1. CPU: WDC W65C02 (S variant required for 3.3v support)
2. RAM: Alliance AS6C62 8k/32k parallel SRAM (or anything that is pin-compatible with the DIP-28 package)
3. ROM: Atmel AT28C64 or AT28C256 for 5v systems, AT28LV or BV versions for 3.3v.
4. VIA (GPIO, timers): WDC W65C22 (S variant required for 3.3v support)
5. ACIA (serial port): Any variant of the 65C51 chip, I like the CDP65C51 since it runs at 3.3v or 5v and doesn't have the [hardware bug](http://forum.6502.org/viewtopic.php?f=4&t=2543&sid=c5f70265c3a9616ffc7b7f75fac0389d) from the WDC version

WDC still produces the 65C02 CPU and the 65C22 VIA which both support 3.3v. Most modern parallel SRAM also supports 3.3v. The hardest pieces to find are ROM and 65C51 ACIAs. There are modern Flash EEPROMs available that will run at 3.3v, but they are expensive and only available in surface packages. WDC's current production version of the 65C51 only operates at 5v. I found some CDP65C51 chips on eBay that will run at 3.3v or 5v. They are available in 1, 2, or 4 MHz varieties.

In summary, if you really want to build a Herring (or any similar 6502 system), stick with 5v if you plan to use any vintage parts and only use 3.3v if you plan to interface your system with modern hardware.

## System Bus

The motherboard and daughterboards are connected with a 40 pin single-row connector. Most of these pins come directly from the 65C02. They are mapped from left to right in the following order:

1. VCC - 5v or 3.3v depending on the backplane's voltage regulator
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
28. CLK_OUT - Clock out provided by the CPU, used to drive peripherals that require the system clock
29. RWB - Read/write flag (Read = high, Write = low)
30. IRQB - Interrupt request (Active low)
31. DEC0
32. DEC1
33. DEC2
34. DEC3
35. DEC4
36. DEC5
37. DEC6
38. DEC7
39. CLK_IN - External clock source for the CPU, provided by an oscillator or by a clock divider on an FPGA
40. GND

# Software

Herring currently runs a simple monitor program from ROM that provides the basic peek, poke, jump commands as well as memory clearing and memory dumping.

```
Herring 6502 Monitor v1.2
Colin Maykish - 2021
github.com/crmaykish/herring-6502
-----
Available RAM: 31581 bytes free
-----
> help
Commands:
help
clear
free
peek <addr>
poke <addr> <val>
dump <addr>
jump <addr>
zero <addr>
```

```
> dump E000
E000  A9 FF 8D 03 84 8D 02 84  A9 00 8D 00 84 8D 01 84  |................|
E010  4C 26 E0 A9 F0 8D 01 84  4C 23 E0 A9 0F 8D 01 84  |L&......L#......|
E020  4C 23 E0 4C 23 E0 78 A2  FF 9A D8 A9 00 85 02 A9  |L#.L#.x.........|
E030  80 85 03 20 B4 EC A9 01  8D 01 84 20 97 E6 A9 02  |... ....... ....|
E040  8D 01 84 20 68 E0 A9 03  8D 01 84 20 D5 E0 A9 3C  |... h...... ...<|
E050  8D 01 84 4C 23 E0 58 E0  38 A5 02 E9 00 8D 06 02  |...L#.X.8.......|
E060  A5 03 E9 02 8D 07 02 60  A0 02 F0 07 A9 56 A2 E0  |.......`.....V..|
E070  4C 0C 02 60 8D 00 02 8E  01 02 6C 00 02 20 01 E6  |L..`......l.. ..|
```

This monitor program is written in C and was built with the cc65 compiler. A customized set of config files are provided in the `firmware/config` directory.

# References

## Inspiration

1. HomeComputer 6502: https://www.grappendorf.net/projects/6502-home-computer/

## Guides

1. How to read SD cards with a 6502: https://github.com/gfoot/sdcard6502
2. ACIA 6551: http://retro.hansotten.nl/6502-sbc/lee-davison-web-site/acia-6551/