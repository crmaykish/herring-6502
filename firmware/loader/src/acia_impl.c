#include "acia.h"

unsigned char ACIA_ReadLine(char *buffer, unsigned char max, bool echo)
{
    unsigned char bytes_read = 0;
    char in = 0;

    while (in != 0x0A && in != 0x0D && bytes_read < max)
    {
        in = ACIA_ReadByte();

        if (echo)
        {
            ACIA_WriteByte(in);
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
        ACIA_WriteByte(buffer[i]);
        i++;
    }
}
