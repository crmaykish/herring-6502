#include "herring.h"
#include "acia.h"
#include "lcd.h"

int main()
{
    print("Setting up LCD... ");
    lcd_init();
    print("DONE\r\n");

    lcd_print("Herring 6502");

    return 0;
}
