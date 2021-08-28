#include "acia.h"

byte readline(char *buffer, bool echo)
{
    byte count = 0;
    byte in = getc();

    while (in != '\n' && in != '\r')
    {
        if (echo)
        {
            putc(in);
        }

        buffer[count] = in;
        in = getc();
        count++;
    }

    buffer[count] = 0;

    return count;
}
