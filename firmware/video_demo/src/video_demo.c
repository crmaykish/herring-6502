#include <string.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

#define FB_ADDR (0x9000)
#define FB_START ((unsigned char*) (FB_ADDR))
#define FB_SIZE (160 * 120)

void delay(word t)
{
    word delay = 0;

    while (delay < t)
    {
        asm("nop");
        delay++;
    }
}

void draw_block(byte x, byte y, byte w, byte h, byte color)
{
    
}

int main()
{
    byte bg_color = 0x01;
    byte player_color = 0x4;


    byte pos_x = 0;
    byte pos_y = 0;

    byte sprite[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02};

    acia_init();

    print_line("Starting framebuffer demo");

    print_line("Clearing framebuffer");
    memset(FB_START, 0, FB_SIZE);
    print_line("Done!");

    print_line("Starting framebuffer demo...");

    memcpy(FB_START + (pos_y * 160) + pos_x, sprite, sizeof(sprite));

    while (true)
    {
        // erase current sprite position
        memset(FB_START + (pos_y * 160) + pos_x, 0, sizeof(sprite));

        pos_x++;
        pos_y++;

        // redraw it at new position
        memcpy(FB_START + (pos_y * 160) + pos_x, sprite, sizeof(sprite));

        delay(20000);
        delay(20000);
    }

    return 0;
}
