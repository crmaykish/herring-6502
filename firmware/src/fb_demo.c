#include "herring.h"
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>

#define VGA_ADDR 0x8000
#define FB_WIDTH 64
#define FB_HEIGHT 48
#define FB_SIZE (FB_WIDTH * FB_HEIGHT)

#define SWAP_BUFFERS() POKE(VGA_ADDR, 0x40)

#define SET_PIXEL(x, y, color) POKE(VGA_ADDR + (y * 64) + x, color)

void fb_clear()
{
    memset((void *)VGA_ADDR, 0, FB_SIZE);
    SWAP_BUFFERS();
    memset((void *)VGA_ADDR, 0, FB_SIZE);
    SWAP_BUFFERS();
}

int main()
{
    int i;

    srand(77);

    fb_clear();

    while (1)
    {
        for (i = 0; i < FB_SIZE; i++)
        {
            POKE(VGA_ADDR + i, rand() % 8);
        }

        SWAP_BUFFERS();
    }

    return 0;
}
