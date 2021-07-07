#include <peekpoke.h>
#include "acia.h"

static unsigned char serial_buffer_idx = 0;
static unsigned char serial_buffer[SERIAL_BUFFER_SIZE];

void ACIA_Init()
{
    POKE(ACIA_STATUS, 0x00);
    POKE(ACIA_COMMAND, 0x0B);
    POKE(ACIA_CONTROL, 0x1E);
}

void ACIA_Write(char c)
{
    while ((PEEK(ACIA_STATUS) & 0x10) == 0)
    {
        // Wait for ACIA Tx ready flag
    }

    // Write the character to the ACIA
    POKE(ACIA_DATA, c);
}

void ACIA_NewLine()
{
    ACIA_Write('\n');
    ACIA_Write('\r');
}

void ACIA_WriteBuffer(char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        ACIA_Write(buffer[i]);
        i++;
    }
}

void ACIA_WriteLine(char *message)
{
    ACIA_WriteBuffer(message);
    ACIA_NewLine();
}

unsigned char ACIA_Read()
{
    while ((PEEK(ACIA_STATUS) & 0x08) == 0)
    {
        // Wait for ACIA Rx data
    }

    return PEEK(ACIA_DATA);
}

void ACIA_ReadLine()
{
    for (;;)
    {
        serial_buffer[serial_buffer_idx] = ACIA_Read();

        // echo back to the console
        ACIA_Write(serial_buffer[serial_buffer_idx]);

        if (serial_buffer[serial_buffer_idx] == '\n' || serial_buffer[serial_buffer_idx] == '\r')
        {
            serial_buffer_idx = 0;
            return;
        }

        serial_buffer_idx++;

        if (serial_buffer_idx == SERIAL_BUFFER_SIZE)
        {
            serial_buffer_idx = 0;
            return;
        }
    }
}

unsigned char *ACIA_GetBuffer()
{
    return serial_buffer;
}