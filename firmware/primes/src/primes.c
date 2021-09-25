#include <stdlib.h>
#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "lcd.h"
#include "acia.h"

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
    byte in = 0;
    char buffer[17] = {0};

    acia_init();
    lcd_init();

    lcd_clear();
    lcd_home();

    while (true)
    {
        readline(buffer, true);
        lcd_print(buffer);
    }

    return 0;
}