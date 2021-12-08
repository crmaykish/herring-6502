#ifndef LCD_H
#define LCD_H

#include "herring.h"

extern void lcd_init();
extern void lcd_clear();
extern void lcd_home();
extern void __fastcall__ lcd_putc(char);
extern void __fastcall__ lcd_print(char *);

void lcd_set_cursor(byte x, byte y);

#endif