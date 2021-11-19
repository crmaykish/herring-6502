#include <string.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

#define FB_ADDR (0x9000)
#define FB_START ((unsigned char *)(FB_ADDR))
#define FB_WIDTH (80)
#define FB_HEIGHT (60)
#define FB_SIZE (FB_WIDTH * FB_HEIGHT)

#define BLACK 0x0
#define PURPLE 0x5
#define WHITE 0x7

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
    byte i = 0;

    for (i; i < h; i++)
    {
        memset(FB_START + ((y + i) * FB_WIDTH) + x, color, w);
    }
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

    print_line("Clearing front and back framebuffers");
    memset(FB_START, 0, FB_SIZE);
    POKE(FB_START, 0b01000000);
    memset(FB_START, 0, FB_SIZE);
    POKE(FB_START, 0b01000000);

    print_line("Done!");

    print_line("Starting framebuffer demo...");

    while (true)
    {
        // clear framebuffer
        memset(FB_START, 0, FB_SIZE);

        // redraw the useful stuff
        draw_block(pos_x, pos_y, 4, 4, PURPLE);

        // draw border
        draw_block(0, 0, FB_WIDTH, 1, WHITE);
        draw_block(0, FB_HEIGHT - 1, FB_WIDTH, 1, WHITE);
        draw_block(0, 0, 1, FB_HEIGHT, WHITE);
        draw_block(FB_WIDTH - 1, 0, 1, FB_HEIGHT, WHITE);

        // swap buffers now that drawing is done
        POKE(FB_START, 0b01000000);

        // Update program state

        pos_x++;
        pos_y++;

        if (pos_x >= FB_WIDTH || pos_y >= FB_HEIGHT)
        {
            pos_x = 0;
            pos_y = 0;
        }

        // Frame timing delay
        delay(2000);
    }

    return 0;
}
