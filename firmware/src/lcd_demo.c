#include "herring.h"
#include "print.h"
#include "lcd.h"

int main()
{
    lcd_init();

    lcd_clear();
    lcd_home();

    lcd_print("Herring 6502");

    lcd_set_cursor(0, 1);

    lcd_print("Hello!");    

    return 0;
}
