#include "herring.h"
#include "acia.h"
#include "lcd.h"

int main()
{
    byte in = 0;

    lcd_create();

    while (in != 0x03)
    {
        in = getc();
        lcd_write(in);
    }

    return 0;
}
