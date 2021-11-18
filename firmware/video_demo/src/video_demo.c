#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

int main()
{
    char input = 0;

    acia_init();

    print_line("Enter characters to send to the display: ");

    while (true)
    {
        input = acia_getc();

        POKE(0x8C00, input - 64);
    }

    return 0;
}