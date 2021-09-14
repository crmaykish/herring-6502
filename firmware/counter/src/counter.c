#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"

#define VGA_SET_X 0x8C00
#define VGA_SET_Y 0x8C01
#define VGA_SET_COLOR 0x8C02
#define VGA_DRAW_PIXEL 0x8C03

#define VGA_BLACK 0b000
#define VGA_RED 0b001
#define VGA_GREEN 0b010
#define VGA_YELLOW 0b011
#define VGA_BLUE 0b100
#define VGA_PURPLE 0b101
#define VGA_CYAN 0b110
#define VGA_WHITE 0b111

void delay(word t)
{
    word delay = 0;

    while (delay < t)
    {
        asm("nop");
        delay++;
    }
}

int main()
{
    byte x = 0;
    byte y = 0;
    byte color = VGA_RED;

    while (true)
    {
        for (x = 0; x < 160; x++)
        {
            POKE(VGA_SET_COLOR, color);

            for (y = 0; y < 120; y++)
            {
                POKE(VGA_SET_X, x);
                POKE(VGA_SET_Y, y);
                POKE(VGA_DRAW_PIXEL, 0);
            }
        }

        if (color == VGA_WHITE)
        {
            color = VGA_BLACK;
        }
        else
        {
            color++;
        }
    }

    return 0;
}
