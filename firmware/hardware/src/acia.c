#include "acia.h"
#include <stdlib.h>

void acia_init()
{
    POKE(ACIA0_STATUS, ACIA_RESET);
    POKE(ACIA0_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA0_CONTROL, ACIA_CONTROL_BAUD_19200);
}

char getc()
{
    while ((PEEK(0xDC01) & 0x08) == 0)
    {
    }
    return PEEK(0xDC00);
}

void putc(char c)
{
    while ((PEEK(0xDC01) & 0x10) == 0)
    {
    }
    POKE(0xDC00, c);
}

byte readline(char *buffer, byte max, bool echo)
{
    byte bytes_read = 0;
    byte in = 0;

    while (in != 0x0A && in != 0x0D && bytes_read < max)
    {
        in = getc();

        if (echo)
        {
            putc(in);
        }

        buffer[bytes_read] = in;
        bytes_read++;
    }

    // NULL-terminate the buffer
    buffer[bytes_read] = 0;

    return bytes_read;
}

void print(char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        putc(buffer[i]);
        i++;
    }
}

void print_hex(word w)
{
    char s[5];
    utoa(w, s, 16);

    if (w < 0x10)
    {
        putc('0');
    }

    print(s);
}

void print_int(word w)
{
    char s[6];
    utoa(w, s, 10);
    print(s);
}
