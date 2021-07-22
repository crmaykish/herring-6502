#include "acia.h"
#include <stdlib.h>

void ACIA_Init(const ACIA_t *acia)
{
    // Reset ACIA
    poke(acia->Status, 0x00);
    // RTS low, transmit interupt disabled, receiver interupt disabled, DTR low
    poke(acia->Command, 0x0B);
    // Set baudrate to 19200, 8 data bits, 1 stop bits
    poke(acia->Control, 0b00011111);
}

byte ACIA_Read(const ACIA_t *acia)
{
    while ((peek(acia->Status) & ACIA_READY_RX) == 0)
    {
    }

    return peek(acia->Data);
}

byte ACIA_ReadLine(const ACIA_t *acia, char *buffer, byte max, bool echo)
{
    byte bytes_read = 0;
    byte in = 0;

    while (in != 0x0A && in != 0x0D && bytes_read < max)
    {
        in = ACIA_Read(acia);

        if (echo)
        {
            ACIA_Write(acia, in);
        }

        buffer[bytes_read] = in;
        bytes_read++;
    }

    // NULL-terminate the buffer
    buffer[bytes_read] = 0;

    return bytes_read;
}

void ACIA_Write(const ACIA_t *acia, char c)
{
    while ((peek(acia->Status) & ACIA_READY_TX) == 0)
    {
        // Wait for ACIA Tx ready flag
    }

    // Write the character to the ACIA
    poke(acia->Data, c);
}

void ACIA_NewLine(const ACIA_t *acia)
{
    ACIA_Write(acia, ASCII_NEWLINE);
    ACIA_Write(acia, ASCII_CARRIAGE_RETURN);
}

void ACIA_WriteBuffer(const ACIA_t *acia, char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        ACIA_Write(acia, buffer[i]);
        i++;
    }
}

void put(char c)
{
    ACIA_Write(&SerialConsole, c);
}

void print(char *string)
{
    ACIA_WriteBuffer(&SerialConsole, string);
}

void print_hex(word w)
{
    char int_s[5];
    itoa(w, int_s, 16);
    if (w < 0x10)
    {
        put('0');
    }
    print(int_s);
}
