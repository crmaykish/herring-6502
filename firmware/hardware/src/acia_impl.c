#include "acia.h"

byte ACIA_ReadLine(char *buffer, byte max, bool echo)
{
    byte bytes_read = 0;
    byte in = 0;

    while (in != 0x0A && in != 0x0D && bytes_read < max)
    {
        in = ACIA_Read();

        if (echo)
        {
            ACIA_Write(in);
        }

        buffer[bytes_read] = in;
        bytes_read++;
    }

    // NULL-terminate the buffer
    buffer[bytes_read] = 0;

    return bytes_read;
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
