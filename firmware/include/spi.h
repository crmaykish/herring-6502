#ifndef SPI_H
#define SPI_H

#include "herring.h"

#define SPI_COMMAND_LENGTH 6

// GO_IDLE_STATE
static byte SPI_COMMAND_0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};

// SEND_IF_COND
static byte SPI_COMMAND_8[] = {0x48, 0x00, 0x00, 0x01, 0xAA, 0x87};

static byte SPI_COMMAND_41[] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x01};

static byte SPI_COMMAND_55[] = {0x77, 0x00, 0x00, 0x00, 0x00, 0x01};

static byte SPI_COMMAND_17[] = {0x51, 0x00, 0x00, 0x00, 0x00, 0x01};

extern void SPI_Init();
extern byte SPI_ReadByte();
extern void fastcall SPI_WriteByte(byte b);
extern byte SPI_WaitResult();

byte SPI_SendCommand(byte *command);

#endif