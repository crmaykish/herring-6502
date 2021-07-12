#ifndef HERRING_H
#define HERRING_H

#include <stdbool.h>

/* ########## COMMANDS ########## */

#define peek(addr) (*(byte *)(addr))
#define poke(addr, val) (*(byte *)(addr) = (val))

/* ########## DATA TYPES ########## */

// 8 bits
typedef unsigned char byte;

// 16 bits
typedef unsigned int word;

#endif
