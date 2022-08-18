#include "vga.h"
#include <string.h>

void fb_clear()
{
    memset((void *)VGA_ADDR, 0, FB_SIZE);
    SWAP_BUFFERS();
    memset((void *)VGA_ADDR, 0, FB_SIZE);
    SWAP_BUFFERS();
}