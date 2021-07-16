#ifndef ACIA_H
#define ACIA_H

#include "herring.h"

#define ACIA1 0
#define ACIA2 1

// User-facing serial terminal
#define ACIA_TERM ACIA1

// Communications serial port for other peripherals
#define ACIA_COMM ACIA2

// Memory mapping
#define ACIA1_DATA 0xDC00
#define ACIA1_STATUS 0xDC01
#define ACIA1_COMMAND 0xDC02
#define ACIA1_CONTROL 0xDC03

#define ACIA2_DATA 0xD800
#define ACIA2_STATUS 0xD801
#define ACIA2_COMMAND 0xD802
#define ACIA2_CONTROL 0xD803

#define ACIA_DATA(P) (P == ACIA1 ? ACIA1_DATA : ACIA2_DATA)
#define ACIA_STATUS(P) (P == ACIA1 ? ACIA1_STATUS : ACIA2_STATUS)
#define ACIA_COMMAND(P) (P == ACIA1 ? ACIA1_COMMAND : ACIA2_COMMAND)
#define ACIA_CONTROL(P) (P == ACIA1 ? ACIA1_CONTROL : ACIA2_CONTROL)

// TODO: just define an ACIA struct and assign one for each ACIA

#define ACIA_READY_RX 0x08
#define ACIA_READY_TX 0x10

#define ASCII_NEWLINE '\n'
#define ASCII_CARRIAGE_RETURN '\r'

void ACIA_Init(byte acia);
byte ACIA_Read(byte acia);
byte ACIA_ReadLine(byte acia, char *buffer, byte max, bool echo);
void ACIA_Write(byte acia, char c);
void ACIA_WriteBuffer(byte acia, char *buffer);
void ACIA_NewLine(byte acia);

#endif
