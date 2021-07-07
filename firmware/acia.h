#ifndef ACIA_H
#define ACIA_H

#define ACIA_DATA 0x6000
#define ACIA_STATUS 0x6001
#define ACIA_COMMAND 0x6002
#define ACIA_CONTROL 0x6003

#define SERIAL_BUFFER_SIZE 32

void ACIA_Init();

void ACIA_Write(char c);
void ACIA_WriteBuffer(char *buffer);
void ACIA_WriteLine(char *message);
void ACIA_NewLine();

unsigned char ACIA_Read();
void ACIA_ReadLine();

unsigned char *ACIA_GetBuffer();

#endif