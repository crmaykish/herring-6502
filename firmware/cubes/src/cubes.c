#include <stdlib.h>
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

#define FB_SWAP_COMMAND (0b01000000)
#define FB_SWAP_BUFFERS() POKE(FB_START, FB_SWAP_COMMAND)

#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define PURPLE 0x5
#define CYAN 0x6
#define WHITE 0x7

#define BALL_COUNT 10

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

static void buffer_swap()
{
    while ((PEEK(FB_START) & 0b1) != 0b0)
    {
        // Wait for the vertical blanking time
    }

    FB_SWAP_BUFFERS();
}

static void buffer_clear()
{
    memset(FB_START, 0, FB_SIZE);
}

void draw_ball(byte x, byte y, byte w, byte h, byte color);

int main()
{
    ball_t balls[BALL_COUNT];

    byte seed = 0;
    byte seed_count = 0;

    byte i = 0;

    acia_init();

    print_line("\r\n|=== VGA Bouncing Cubes Demo === |\r\n");

    print_line("Clearing front and back framebuffers");

    buffer_clear();
    buffer_swap();
    buffer_clear();
    buffer_swap();

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
        balls[i].x_dir = ((rand() % 1) ? -1 : 1) * ((rand() % 2) + 1);
        balls[i].y_dir = ((rand() % 1) ? -1 : 1) * ((rand() % 2) + 1);
        balls[i].color = (i % 7) + 1;
    }

    print_line("Starting VGA demo...");

    while (true)
    {
        // === Erase the framebuffer === //

        buffer_clear();

        // === Update the game state === //

        for (i = 0; i < BALL_COUNT; i++)
        {
            balls[i].pos_x += balls[i].x_dir;
            balls[i].pos_y += balls[i].y_dir;

            if (balls[i].pos_x <= 0 || balls[i].pos_x >= (FB_WIDTH - balls[i].width))
            {
                balls[i].x_dir *= -1;
            }

            if (balls[i].pos_y <= 0 || balls[i].pos_y >= (FB_HEIGHT - balls[i].height))
            {
                balls[i].y_dir *= -1;
            }
        }

        // === Draw the updated framebuffer === //

        for (i = 0; i < BALL_COUNT; i++)
        {
            draw_ball(balls[i].pos_x, balls[i].pos_y, balls[i].width, balls[i].height, balls[i].color);
        }

        // === Swap display buffers === ///

        buffer_swap();
    }

    return 0;
}

void draw_ball(byte x, byte y, byte w, byte h, byte color)
{
    byte i = 0;

    for (i; i < h; i++)
    {
        memset(FB_START + ((y + i) * FB_WIDTH) + x, color, w);
    }
}