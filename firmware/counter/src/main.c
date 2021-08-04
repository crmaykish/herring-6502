#include "herring.h"
#include "acia.h"
#include "lcd.h"

int main()
{
    print("Setting up LCD... ");
    lcd_init();
    print("DONE\r\n");



    lcd_print("Herring 6502");
    lcd_line2();
    lcd_print("new line");

    return 0;
}
