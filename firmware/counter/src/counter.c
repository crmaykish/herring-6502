#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"

#define VGA_SET_X 0x8C00
#define VGA_SET_Y 0x8C01
#define VGA_SET_COLOR 0x8C02
#define VGA_DRAW_PIXEL 0x8C03

#define VGA_WIDTH 200
#define VGA_HEIGHT 150

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

void vga_draw_pixel(byte x, byte y)
{
    POKE(VGA_SET_X, x);
    POKE(VGA_SET_Y, y);
    POKE(VGA_DRAW_PIXEL, 0);
}

void vga_set_color(byte color)
{
    POKE(VGA_SET_COLOR, color);
}

void vga_clear()
{
    byte x = 0;
    byte y = 0;

    vga_set_color(VGA_BLACK);

    for (x = 0; x < VGA_WIDTH; x++)
    {
        for (y = 0; y < VGA_HEIGHT; y++)
        {
            POKE(VGA_SET_X, x);
            POKE(VGA_SET_Y, y);
            POKE(VGA_DRAW_PIXEL, 0);
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

int main()
{
    byte x = 0;
    byte y = 0;
    byte i = 0;
    byte color = VGA_RED;

    byte in = 0;

    byte enemies[4][2] = {{10, 10}, {40, 10}, {70, 10}, {100, 10}};

    bool enemy_dir = true;

    acia_init();

    vga_clear();

    // draw a border
    vga_set_color(VGA_BLUE);
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

    // Game loop
    while (true)
    {

        if (acia_rx_ready() != 0)
        {
            in = acia_getc();

            vga_set_color(VGA_BLACK);
            vga_draw_pixel(x, y);
            vga_draw_pixel(x, y + 1);
            vga_draw_pixel(x + 1, y);
            vga_draw_pixel(x + 1, y + 1);

            if (in == 'w')
            {
                if (y > 2)
                {
                    y -= 2;
                }
            }
            else if (in == 'a')
            {
                if (x > 2)
                {
                    x -= 2;
                }
            }
            else if (in == 's')
            {
                if (y < VGA_HEIGHT - 4)
                {
                    y += 2;
                }
            }
            else if (in == 'd')
            {
                if (x < VGA_WIDTH - 4)
                {
                    x += 2;
                }
            }
        }

        vga_set_color(VGA_GREEN);
        vga_draw_pixel(x, y);
        vga_draw_pixel(x, y + 1);
        vga_draw_pixel(x + 1, y);
        vga_draw_pixel(x + 1, y + 1);

        // Update and draw enemies

        for (i = 0; i < 4; i++)
        {
            vga_set_color(VGA_BLACK);
            // vga_draw_pixel(enemies[i][0], enemies[i][1]);
            draw_block(enemies[i][0], enemies[i][1], 5, 2);

            // Update positions
            enemies[i][0] += (byte)enemy_dir;

            vga_set_color(VGA_RED);
            // vga_draw_pixel(enemies[i][0], enemies[i][1]);
            draw_block(enemies[i][0], enemies[i][1], 5, 2);
        }

        if (enemies[3][0] == VGA_WIDTH - 8 || enemies[0][0] == 4)
        {
            enemy_dir = ~enemy_dir;
        }

        delay(1000);
        // TODO: needs a real timer to control framerate
    }

    return 0;
}
