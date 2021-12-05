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

void draw_line(byte x1, byte y1, byte x2, byte y2, byte color)
{
    // THIS ONLY WORKS IN OCTANT 1
    int x;
    int y = y1;
    int m_new = 2 * (y2 - y1);
    int slope_error_new = m_new - (x2 - x1);
    for (x = x1; x <= x2; x++)
    {
        draw_pixel(x, y, color);

        // Add slope to increment angle formed
        slope_error_new += m_new;

        // Slope error reached limit, time to
        // increment y and update slope error.
        if (slope_error_new >= 0)
        {
            y++;
            slope_error_new -= 2 * (x2 - x1);
        }
    }
}

void fill_rect(byte x, byte y, byte width, byte height, byte color)
{
    byte i = 0;

    for (i = 0; i < height; i++)
    {
        memset(pixel_address(x, y + i), color, width);
    }
}
