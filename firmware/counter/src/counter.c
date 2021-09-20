#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

#define VGA_STATUS 0x9003
#define VGA_SET_COLOR 0x9000
#define VGA_SET_X 0x9001
#define VGA_SET_Y 0x9002
#define VGA_SET_PIXEL 0x00

#define VGA_WIDTH 160
#define VGA_HEIGHT 120

#define VGA_BLACK 0b000
#define VGA_RED 0b001
#define VGA_GREEN 0b010
#define VGA_YELLOW 0b011
#define VGA_BLUE 0b100
#define VGA_PURPLE 0b101
#define VGA_CYAN 0b110
#define VGA_WHITE 0b111

// {0b, 0b, 0b, 0b, 0b},

byte digits[10][5] = {
    {0b01111110, 0b10000001, 0b10000001, 0b10000001, 0b01111110}, // 0
    {0b10000000, 0b10000010, 0b11111111, 0b10000000, 0b10000000}, // 1
    {0b11000010, 0b10100001, 0b10010001, 0b10001001, 0b10000110}, // 2
    {0b01000010, 0b10000001, 0b10001001, 0b10001001, 0b01110110}, // 3
    {0b00010000, 0b00011000, 0b00010100, 0b00010010, 0b11111111}, // 4
    {0b01001111, 0b10001001, 0b10001001, 0b10001001, 0b01110001}, // 5
    {0b01111100, 0b10010010, 0b10010001, 0b10010001, 0b01100000}, // 6
    {0b00000001, 0b00000001, 0b11111001, 0b00000101, 0b00000011}, // 7
    {0b01110110, 0b10001001, 0b10001001, 0b10001001, 0b01110110}, // 8
    {0b00000110, 0b10001001, 0b10001001, 0b01001001, 0b00111110}, // 9
};

void delay(word t)
{
    word delay = 0;

    while (delay < t)
    {
        asm("nop");
        delay++;
    }
}

void vga_draw_pixel(byte x, byte y)
{
    POKE(VGA_SET_X, x);
    POKE(VGA_SET_Y, y);
    POKE(VGA_STATUS, VGA_SET_PIXEL);
}

void vga_clear()
{
    byte x = 0;
    byte y = 0;

    POKE(VGA_SET_COLOR, VGA_BLACK);

    for (x = 0; x < VGA_WIDTH; x++)
    {
        POKE(VGA_SET_X, x);

        for (y = 0; y < VGA_HEIGHT; y++)
        {
            POKE(VGA_SET_Y, y);
            POKE(VGA_STATUS, VGA_SET_PIXEL);
        }
    }
}

void draw_block(byte x, byte y, byte w, byte h)
{
    byte i = 0;
    byte j = 0;

    for (i = 0; i < w; i++)
    {
        for (j = 0; j < h; j++)
        {
            vga_draw_pixel(x + i, y + j);
        }
    }
}

void draw_char(byte c[], byte x, byte y)
{
    byte i = 0;
    byte j = 0;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (c[i] & (1 << j))
            {
                vga_draw_pixel(x + i, y + j);
            }
        }
    }
}

void draw_border()
{
    byte x = 0;
    byte y = 0;

    // draw a border
    POKE(VGA_SET_COLOR, VGA_BLUE);
    for (x = 0; x < VGA_WIDTH; x++)
    {
        vga_draw_pixel(x, 0);
        vga_draw_pixel(x, VGA_HEIGHT - 1);
    }

    for (y = 0; y < VGA_HEIGHT; y++)
    {
        vga_draw_pixel(0, y);
        vga_draw_pixel(VGA_WIDTH - 1, y);
    }

    x = VGA_WIDTH / 2;
    y = VGA_HEIGHT - 8;
}

void vga_print(word num, byte x, byte y)
{
    byte i = 0;
    byte j = 0;
    byte d[5];
    bool ready = false;

    while (num)
    {
        d[i] = (byte)(num % 10);
        num /= 10;
        i++;
    }

    for (i = 0; i < 5; i++)
    {
        if (d[4 - i] != 0 || ready)
        {
            ready = true;
            draw_char(digits[d[4 - i]], x + j, y);
            j += 6;
        }
    }
}

int main()
{
    byte x = 0;
    byte y = 0;
    byte i = 0;
    byte color = VGA_RED;

    word counter = 0;
    byte counter_rollover = 0;

    draw_border();

    // Game loop
    while (true)
    {
        vga_clear();

        

        if (counter_rollover == 10)
        {
            POKE(VGA_SET_COLOR, VGA_WHITE);
            vga_print(counter, 10, VGA_HEIGHT - 10 - 8);

            counter++;
            counter_rollover = 0;
        }

        counter_rollover++;
    }

    return 0;
}
