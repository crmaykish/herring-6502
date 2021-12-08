#include <peekpoke.h>
#include "herring.h"
#include "acia.h"

#define LCD_COMMAND_REG (0x9000)

// TODO: This would be better implemented in assembly
void lcd_set_cursor(byte x, byte y)
{
    POKE(LCD_COMMAND_REG, 0b10000000 | (y == 0 ? 0 : 0x40) | x);
}
