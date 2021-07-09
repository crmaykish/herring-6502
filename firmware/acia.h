#ifndef ACIA_H
#define ACIA_H

#include <stdbool.h>

#define SERIAL_BUFFER_SIZE 40

void ACIA_Init();

void ACIA_Write(char c);
void ACIA_WriteBuffer(char *buffer);
void ACIA_WriteLine(char *message);
void ACIA_NewLine();

bool ACIA_DataAvailable();
unsigned char ACIA_Read();
void ACIA_ReadLine();

unsigned char *ACIA_GetBuffer();

void ACIA_ClearBuffer();

#endif
