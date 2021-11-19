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

#define BALL_COUNT 6

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
    ball_t balls[BALL_COUNT] = {
        {0, 0, 3, 4, 2, 1, RED},
        {10, 20, 2, 2, -1, 1, GREEN},
        {40, 5, 5, 5, 1, -1, YELLOW},
        {20, 40, 2, 3, -1, -1, BLUE},
        {0, 30, 4, 2, 1, -1, PURPLE},
        {10, 50, 4, 4, -1, 1, CYAN}};

    acia_init();

    print_line("Starting framebuffer demo");

    print_line("Clearing front and back framebuffers");

    buffer_clear();
    buffer_swap();
    buffer_clear();
    buffer_swap();

    print_line("Done!");

    print_line("Starting framebuffer demo...");

    while (true)
    {
        byte i = 0;

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