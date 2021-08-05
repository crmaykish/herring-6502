#ifndef LCD_H
#define LCD_H

#include "herring.h"

#define LCD_ROW_COUNT 2
#define LCD_COL_COUNT 16

typedef struct
{
    char rows[LCD_ROW_COUNT + 1][LCD_COL_COUNT + 1];
    byte current_row;
    byte char_count;
} lcd_t;

void lcd_create();
void lcd_write(byte);
void lcd_print(char *);
void lcd_reset();

#endif
