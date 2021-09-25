#include <stdlib.h>
#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "lcd.h"

// very slow!
bool is_prime(word p)
{
    word w = 3;

    if (p % 2 == 0)
    {
        return false;
    }

    while (w < p / 2)
    {
        if (p % w == 0)
        {
            return false;
        }

        w += 2;
    }

    return true;
}

int main()
{
    acia_init();

    lcd_init();

    lcd_putc('H');
    lcd_putc('e');
    lcd_putc('r');
    lcd_putc('r');
    lcd_putc('i');
    lcd_putc('n');
    lcd_putc('g');

    while (true)
    {
    }

    return 0;
}