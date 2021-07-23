#ifndef HERRING_H
#define HERRING_H

/* ########## MEMORY ########## */
#define PROGRAM_RAM 0x1000
#define ROM 0xE000
#define ROM_PROGRAM (ROM + 0x25)

/* ########## COMMANDS ########## */

#define peek(addr) (*(byte *)(addr))
#define poke(addr, val) (*(byte *)(addr) = (val))

/* ########## DATA TYPES ########## */

// 8 bits
typedef unsigned char byte;

// 16 bits
typedef unsigned int word;

#endif
