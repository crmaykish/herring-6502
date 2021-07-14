#include <stdlib.h>
#include "utils.h"
#include "acia.h"

void PrintInt(word w)
{
    char int_s[5];
    itoa(w, int_s, 16);
    ACIA_WriteBuffer(int_s);
}

byte HexToInt(char hex)
{
    if (hex >= '0' && hex <= '9')
    {
        return (byte)(hex - '0');
    }
    else if (hex >= 'A' && hex <= 'F')
    {
        return (byte)(hex - 55);
    }

    return 0;
}

word IntegerValue(char *buffer)
{
    word total = 0;
    byte i = 0;
    byte digits = 0;

    // count the number of digits in the string
    while (buffer[digits] != 0 &&
           buffer[digits] != 0x0D &&
           buffer[digits] != 0x0A)
    {
        digits++;
    }

    if (digits > 4)
    {
        return 0;
    }

    for (i = 0; i < digits; i++)
    {
        total += (HexToInt(buffer[i]) << (4 * (digits - i - 1)));
    }

    return total;
}