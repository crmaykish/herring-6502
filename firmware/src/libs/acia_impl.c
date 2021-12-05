#include "acia.h"

byte readline(char *buffer, bool echo)
{
    byte count = 0;
    byte in = acia_getc();

    while (in != '\n' && in != '\r')
    {
        if (echo)
        {
            acia_putc(in);
        }

        buffer[count] = in;
        in = acia_getc();
        count++;
    }

    buffer[count] = 0;

    return count;
}
