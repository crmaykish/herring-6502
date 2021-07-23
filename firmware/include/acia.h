#ifndef ACIA_H
#define ACIA_H

#include "herring.h"
#include <stdbool.h>

// Memory mapping
#define ACIA0_DATA 0xDC00
#define ACIA0_STATUS 0xDC01
#define ACIA0_COMMAND 0xDC02
#define ACIA0_CONTROL 0xDC03

#define ACIA1_DATA 0xD800
#define ACIA1_STATUS 0xD801
#define ACIA1_COMMAND 0xD802
#define ACIA1_CONTROL 0xD803

#define ACIA_READY_RX 0x08
#define ACIA_READY_TX 0x10

#define ASCII_NEWLINE '\n'
#define ASCII_CARRIAGE_RETURN '\r'

typedef struct
{
    word Data;
    word Status;
    word Command;
    word Control;
} ACIA_t;

static const ACIA_t SerialConsole = {ACIA0_DATA, ACIA0_STATUS, ACIA0_COMMAND, ACIA0_CONTROL};
// static const ACIA_t SerialPeripheral = {ACIA1_DATA, ACIA1_STATUS, ACIA1_COMMAND, ACIA1_CONTROL};

void ACIA_Init(const ACIA_t *acia);
byte ACIA_Read(const ACIA_t *acia);
byte ACIA_ReadLine(const ACIA_t *acia, char *buffer, byte max, bool echo);
void ACIA_Write(const ACIA_t *acia, char c);
void ACIA_WriteBuffer(const ACIA_t *acia, char *buffer);
void ACIA_NewLine(const ACIA_t *acia);

// Output functions, these all write to the serial console
void put(char c);
void print(char *string);
void print_hex(word w);

#endif
