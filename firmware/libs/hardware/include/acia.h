#ifndef ACIA_H
#define ACIA_H

#include <stdbool.h>
#include "herring.h"

extern void acia_init();

extern char acia_rx_ready();
extern char acia_getc();
extern void __fastcall__ acia_putc(byte);
extern void __fastcall__ print(byte *);

byte readline(char *buffer, bool echo);

#endif
