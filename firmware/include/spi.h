#ifndef SPI_H
#define SPI_H

#include "herring.h"

extern void SPI_Init();
extern byte SPI_ReadByte();
extern void __fastcall__ SPI_WriteByte(byte b);
extern byte SPI_WaitResult();

#endif