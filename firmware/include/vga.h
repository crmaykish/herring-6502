#ifndef VGA_H
#define VGA_H

#include "herring.h"
#include <peekpoke.h>

#define VGA_BLACK 0
#define VGA_RED 1
#define VGA_GREEN 2
#define VGA_YELLOW 3
#define VGA_BLUE 4
#define VGA_MAGENTA 5
#define VGA_CYAN 6
#define VGA_WHITE 7

// Framebuffer Mode

#define FB_WIDTH 64
#define FB_HEIGHT 48
#define FB_SIZE (FB_WIDTH * FB_HEIGHT)

#define FB_SWAP_BUFFERS() POKE(VGA0, 0x40)
#define FB_SET_PIXEL(x, y, color) POKE(VGA0 + (y * 64) + x, color)

void fb_clear();

// Text Mode

#define TEXTMODE_WRITE_CHAR(a) POKE(VGA_TEXTMODE_DATA, a)
#define TEXTMODE_SET_COLOR(c) POKE(VGA_TEXTMODE_SET_COLOR_COMMAND, c)

void textmode_clear();

#endif