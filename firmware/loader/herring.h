#ifndef HERRING_H
#define HERRING_H

/* ########## MEMORY MAP ########## */

// Via 1 (parallel port)
#define VIA1_PORTB 0xC400
#define VIA1_PORTA 0xC401
#define VIA1_DDRB 0xC402
#define VIA1_DDRA 0xC403

// ACIA 1 (serial port)
#define ACIA1_DATA 0xDC00
#define ACIA1_STATUS 0xDC01
#define ACIA1_COMMAND 0xDC02
#define ACIA1_CONTROL 0xDC03

/* ########## COMMANDS ########## */

#define peek(addr) (*(byte *)(addr))
#define poke(addr, val) (*(byte *)(addr) = (val))

/* ########## DATA TYPES ########## */

// 8 bits
typedef unsigned char byte;

// 16 bits
typedef unsigned int word;

#endif
