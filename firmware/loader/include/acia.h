#ifndef ACIA_H
#define ACIA_H

#include "herring.h"
#include <stdlib.h>

extern void ACIA_Init();
extern void __fastcall__ ACIA_Write(char c);
extern void ACIA_NewLine();
extern byte ACIA_Read();

byte ACIA_ReadLine(char *buffer, byte max, bool echo);
void ACIA_WriteBuffer(char *buffer);

#endif
