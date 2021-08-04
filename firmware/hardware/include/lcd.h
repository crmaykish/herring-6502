#ifndef LCD_H
#define LCD_H

#include "herring.h"

void lcd_init();
void __fastcall__ lcd_putc(byte);
void lcd_clear();

void lcd_line2();

void lcd_print(char *);

#endif
