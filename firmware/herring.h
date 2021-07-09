#ifndef HERRING_H
#define HERRING_H

/* ########## MEMORY MAP ########## */

#define RAM_LOW 0x0000
#define RAM_HIGH 0x8000
#define ROM 0xE000

#define RAM_SIZE 0xC000
#define ROM_SIZE 0x2000

// Via 1 (parallel port)
#define VIA1_PORTB 0xC000
#define VIA1_PORTA 0xC001
#define VIA1_DDRB 0xC002
#define VIA1_DDRA 0xC003

// ACIA 1 (serial port)
#define ACIA1_DATA 0xC400
#define ACIA1_STATUS 0xC401
#define ACIA1_COMMAND 0xC402
#define ACIA1_CONTROL 0xC403

/* ########## COMMANDS ########## */

/* ########## DATA TYPES ########## */

// 8 bits
typedef unsigned char byte;

// 16 bits
typedef unsigned int word;

#endif
