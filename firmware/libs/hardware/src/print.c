#include <stdlib.h>
#include "print.h"
#include "acia.h"

void print_hex(word w)
{
    char s[5] = {0};
    utoa(w, s, 16);

    if (w < 0x10)
    {
        acia_putc('0');
    }

    print(s);
}

void print_dec(word w)
{
    char s[6] = {0};
    utoa(w, s, 10);
    print(s);
}

void print_string_bin(char *str, byte max)
{
    byte i = 0;

    while (i < max)
    {
        if (str[i] >= 32 && str[i] < 127)
        {
            acia_putc(str[i]);
        }
        else
        {
            acia_putc('.');
        }

        i++;
    }
}