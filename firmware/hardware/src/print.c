#include <stdlib.h>
#include "print.h"
#include "acia.h"

void print_hex(word w)
{
    char s[5] = {0};
    utoa(w, s, 16);

    if (w < 0x10)
    {
        putc('0');
    }

    print(s);
}

void print_dec(word w)
{
    char s[6] = {0};
    utoa(w, s, 10);
    print(s);
}