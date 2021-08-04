#include "lcd.h"

void lcd_print(char *str)
{
    byte i = 0;
    while (str[i] != 0)
    {
        lcd_putc(str[i]);
        i++;
    }
}
