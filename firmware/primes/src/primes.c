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

void lcd_set_cursor(byte x, byte y)
{
    // TODO: implement this in ASM
    POKE(0x8800, 0b10000000 | (y == 0 ? 0 : 0x40) | x);
}

int main()
{
    byte in = 0;
    char buffer[17] = {0};

    acia_init();
    lcd_init();

    // LCD is not getting reset properly every time
    // Sometimes a power cycle is required to clear it

    lcd_clear();
    lcd_home();

    lcd_print("Herring 6502 >");

    lcd_set_cursor(3, 1);
    lcd_print("line 2");

    while (true)
    {
        readline(buffer, true);

        lcd_clear();
        lcd_home();

        lcd_print(buffer);
    }

    return 0;
}