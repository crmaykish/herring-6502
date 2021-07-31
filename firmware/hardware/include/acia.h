#ifndef ACIA_H
#define ACIA_H

#include "herring.h"

extern void acia_init();

extern char getc();
extern void __fastcall__ putc(byte);
extern void __fastcall__ print(byte *);

byte readline(char *buffer, bool echo);

#endif
