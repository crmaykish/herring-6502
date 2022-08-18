#ifndef VGA_H
#define VGA_H

#include <peekpoke.h>

#define VGA_ADDR 0x8000
#define FB_WIDTH 64
#define FB_HEIGHT 48
#define FB_SIZE (FB_WIDTH * FB_HEIGHT)

#define SWAP_BUFFERS() POKE(VGA_ADDR, 0x40)
#define SET_PIXEL(x, y, color) POKE(VGA_ADDR + (y * 64) + x, color)

void fb_clear();

#endif