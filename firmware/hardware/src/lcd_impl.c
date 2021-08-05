#include "lcd.h"
#include <string.h>

static lcd_t lcd;

extern void lcd_init();
extern void lcd_clear();
extern void __fastcall__ lcd_putc(byte);
extern void lcd_line2();

static void update();

void lcd_create()
{
    lcd_init();
    lcd_reset();
}

void lcd_write(byte b)
{
    if (b == '\r')
    {
        lcd.char_count = 16; // Kind of a hacky way to handle returns
    }
    else if (b != '\n')
    {
        lcd_putc(b);
        lcd.rows[lcd.current_row][lcd.char_count] = b;
        lcd.char_count++;
    }

    update();
}

void lcd_print(char *str)
{
    byte i = 0;
    while (str[i] != 0)
    {
        lcd_write(str[i]);
        i++;
    }
}

void lcd_reset()
{
    lcd.current_row = 0;
    lcd.char_count = 0;

    memset(lcd.rows, 0, (LCD_ROW_COUNT * (LCD_COL_COUNT + 1)));

    lcd_clear();
}

static void update()
{
    if (lcd.char_count == 16)
    {
        if (lcd.current_row == 0)
        {
            lcd_line2();
            lcd.current_row = 1;
        }
        else
        {
            strncpy(lcd.rows[0], lcd.rows[1], LCD_COL_COUNT + 1);
            memset(lcd.rows[1], 0, LCD_COL_COUNT + 1);
        }

        lcd_clear();
        lcd_print(lcd.rows[0]);
        lcd_line2();

        lcd.char_count = 0;
    }
}