#ifndef ACIA_H
#define ACIA_H

#include "herring.h"

#define ACIA1_DATA 0xDC00
#define ACIA1_STATUS 0xDC01
#define ACIA1_COMMAND 0xDC02
#define ACIA1_CONTROL 0xDC03

#define ACIA_READY_RX 0x08
#define ACIA_READY_TX 0x10

#define ASCII_NEWLINE '\n'
#define ASCII_CARRIAGE_RETURN '\r'

void ACIA_Init();
byte ACIA_Read();
byte ACIA_ReadLine(char *buffer, byte max, bool echo);
void ACIA_Write(char c);
void ACIA_WriteBuffer(char *buffer);
void ACIA_NewLine();

#endif
