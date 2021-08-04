#ifndef LCD_H
#define LCD_H

#include "herring.h"

void __fastcall__ lcd_init();
void __fastcall__ lcd_putc(byte);

void lcd_print(char *);

#endif
