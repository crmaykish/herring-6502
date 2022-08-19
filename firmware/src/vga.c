#include "vga.h"
#include "delay.h"
#include <string.h>

void fb_clear()
{
    memset((void *)VGA_FB_DATA, 0, FB_SIZE);
    FB_SWAP_BUFFERS();
    memset((void *)VGA_FB_DATA, 0, FB_SIZE);
    FB_SWAP_BUFFERS();
}

void textmode_clear()
{
    POKE(VGA_TEXTMODE_CLEAR_COMMAND, 0x00);
    delay(20);
}
