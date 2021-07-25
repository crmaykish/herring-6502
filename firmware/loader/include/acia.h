#ifndef ACIA_H
#define ACIA_H

#include <stdbool.h>

extern void ACIA_Init();
extern char ACIA_ReadByte();
extern void __fastcall__ ACIA_WriteByte(char c);

unsigned char ACIA_ReadLine(char *buffer, unsigned char max, bool echo);
void ACIA_WriteBuffer(char *buffer);

#endif
