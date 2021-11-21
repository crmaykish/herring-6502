#include <peekpoke.h>
#include "framebuffer.h"

void framebuffer_init()
{
    framebuffer_clear();
    framebuffer_swap();
    framebuffer_clear();
    framebuffer_swap();
}

void framebuffer_clear()
{
    memset(FB_START, 0, FB_SIZE);
}

void framebuffer_swap()
{
    while ((PEEK(FB_START) & 0b1) != 0b0)
    {
        // Wait for the vertical blanking time
    }

    FB_SWAP_BUFFERS();
}

word *pixel_address(byte x, byte y)
{
    return (word *)(FB_ADDR + (y * FB_WIDTH) + x);
}

void draw_pixel(byte x, byte y, byte color)
{
    POKE(pixel_address(x, y), color);
}

void fill_rect(byte x, byte y, byte width, byte height, byte color)
{
    byte i = 0;

    for (i = 0; i < height; i++)
    {
        memset(pixel_address(x, y + i), color, width);
    }
}
