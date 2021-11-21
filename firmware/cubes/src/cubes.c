#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"
#include "framebuffer.h"

#define BALL_COUNT 7

typedef struct
{
    int pos_x;
    int pos_y;
    int width;
    int height;
    int x_dir;
    int y_dir;
    byte color;
} ball_t;

int main()
{
    ball_t balls[BALL_COUNT];

    byte seed = 0;
    byte seed_count = 0;

    byte i = 0;

    acia_init();

    print_line("\r\n|=== VGA Bouncing Cubes Demo === |\r\n");

    print_line("Clearing front and back framebuffers");

    framebuffer_init();

    print_line("Enter some random characters: ");

    while (seed_count < 10)
    {
        seed ^= acia_getc();
        seed <<= 1;
        acia_putc('.');
        seed_count++;
    }

    print_newline();

    srand(seed);

    for (i = 0; i < BALL_COUNT; i++)
    {
        balls[i].width = (rand() % 8) + 3;  // 3-10
        balls[i].height = (rand() % 8) + 3; // 3-10
        balls[i].pos_x = rand() % (FB_WIDTH - balls[i].width - 1);
        balls[i].pos_y = rand() % (FB_HEIGHT - balls[i].height - 1);
        balls[i].x_dir = ((rand() % 1) ? -1 : 1);
        balls[i].y_dir = ((rand() % 1) ? -1 : 1);
        balls[i].color = (i % 7) + 1;
    }

    print_line("Starting VGA demo...");

    while (true)
    {
        // === Erase the framebuffer === //

        framebuffer_clear();

        // === Update the game state === //

        for (i = 0; i < BALL_COUNT; i++)
        {
            balls[i].pos_x += balls[i].x_dir;
            balls[i].pos_y += balls[i].y_dir;

            if (balls[i].pos_x == 1 || balls[i].pos_x >= (FB_WIDTH - balls[i].width) - 1)
            {
                balls[i].x_dir *= -1;
            }

            if (balls[i].pos_y == 1 || balls[i].pos_y >= (FB_HEIGHT - balls[i].height) - 1)
            {
                balls[i].y_dir *= -1;
            }
        }

        // === Draw the updated framebuffer === //

        for (i = 0; i < BALL_COUNT; i++)
        {
            fill_rect(balls[i].pos_x, balls[i].pos_y, balls[i].width, balls[i].height, balls[i].color);
        }

        // === Swap display buffers === ///

        framebuffer_swap();
    }

    return 0;
}
