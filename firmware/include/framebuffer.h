#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <string.h>
#include "herring.h"

#define FB_ADDR (0x9000)
#define FB_START ((unsigned char *)(FB_ADDR))
#define FB_WIDTH (64)
#define FB_HEIGHT (48)
#define FB_SIZE (FB_WIDTH * FB_HEIGHT)

#define FB_SWAP_COMMAND (0b01000000)
#define FB_SWAP_BUFFERS() POKE(FB_START, FB_SWAP_COMMAND)

#define FB_BLACK 0x0
#define FB_RED 0x1
#define FB_GREEN 0x2
#define FB_YELLOW 0x3
#define FB_BLUE 0x4
#define FB_PURPLE 0x5
#define FB_CYAN 0x6
#define FB_WHITE 0x7

void framebuffer_init();
void framebuffer_clear();
void framebuffer_swap();

word *pixel_address(byte x, byte y);

void draw_pixel(byte x, byte y, byte color);

void draw_line(byte x1, byte y1, byte x2, byte y2, byte color);

void fill_rect(byte x, byte y, byte width, byte height, byte color);

#endif