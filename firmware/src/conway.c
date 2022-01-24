#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "herring.h"
#include "serial.h"

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 20
#define MAX_GENERATIONS 100
#define RANDOM_SEED_FILL 25

static uint8_t current[BOARD_WIDTH][BOARD_HEIGHT];
static uint8_t previous[BOARD_WIDTH][BOARD_HEIGHT];

static uint8_t cycles;
static uint16_t seed;
static uint8_t i, j;
static uint8_t in;

void init_board();
void draw_board();
void update_state();
uint8_t count_neighbors(uint8_t x, uint8_t y);
void rand_prompt();

int main()
{
    printf("Conway's Game of Life\r\n");
    rand_prompt();

    term_cursor_set_vis(false);
    term_clear();

    init_board();

    while (cycles <= MAX_GENERATIONS)
    {
        if (serial_byte_available())
        {
            in = serial_getc();

            if (in == ASCII_ESC)
            {
                term_clear();
                term_cursor_set_vis(true);
                return 0;
            }
        }

        update_state();
        draw_board();
        cycles++;
    }
}

void init_board()
{
    cycles = 1;

    for (j = 0; j < BOARD_HEIGHT; ++j)
    {
        for (i = 0; i < BOARD_WIDTH; ++i)
        {
            previous[i][j] = ((rand() % 100) < RANDOM_SEED_FILL) ? 1 : 0;
        }
    }
}

void draw_board()
{
    term_cursor_set_pos(0, 0);

    term_set_color(TERM_BG_MAGENTA);
    printf("Conway's Game of Life\r\n");
    term_set_color(TERM_RESET);

    for (j = 0; j < BOARD_HEIGHT; ++j)
    {
        for (i = 0; i < BOARD_WIDTH; ++i)
        {
            if (current[i][j] == 1)
            {
                term_set_color(TERM_BG_BRIGHT_GREEN);
                serial_putc(' ');
                serial_putc(' ');
            }
            else
            {
                term_set_color(TERM_RESET);
                serial_putc(' ');
                serial_putc(' ');
            }
        }

        printf("\r\n");
    }

    term_set_color(TERM_BG_MAGENTA);
    printf("Generations: %d", cycles);
    term_set_color(TERM_RESET);
}

void update_state()
{
    uint8_t neighbors;

    // Clear current board
    memset(current, 0, BOARD_WIDTH * BOARD_HEIGHT);

    for (j = 0; j < BOARD_HEIGHT; ++j)
    {
        for (i = 0; i < BOARD_WIDTH; ++i)
        {
            neighbors = count_neighbors(i, j);

            if (neighbors == 3 && previous[i][j] == 0)
            {
                // Cell comes alive
                current[i][j] = 1;
            }
            else if ((neighbors == 2 || neighbors == 3) && previous[i][j] == 1)
            {
                // Cell stays alive
                current[i][j] = 1;
            }
            else
            {
                // Cell dies
                current[i][j] = 0;
            }
        }
    }

    // Copy current board to previous
    memcpy(previous, current, BOARD_WIDTH * BOARD_HEIGHT);
}

uint8_t count_neighbors(uint8_t x, uint8_t y)
{
    // Living neighbors
    uint8_t living = 0;

    if (x > 0 && y > 0)
        living += previous[x - 1][y - 1];

    if (x > 0)
        living += previous[x - 1][y];

    if (x > 0 && y < BOARD_HEIGHT - 1)
        living += previous[x - 1][y + 1];

    if (y > 0)
        living += previous[x][y - 1];

    if (y < BOARD_HEIGHT - 1)
        living += previous[x][y + 1];

    if (x < BOARD_WIDTH - 1 && y > 0)
        living += previous[x + 1][y - 1];

    if (x < BOARD_WIDTH - 1)
        living += previous[x + 1][y];

    if (x < BOARD_WIDTH - 1 && y < BOARD_HEIGHT - 1)
        living += previous[x + 1][y + 1];

    return living;
}

void rand_prompt()
{
    uint8_t seed_count = 0;

    printf("Enter some random characters:\r\n");

    while (seed_count < 10)
    {
        seed += serial_getc();
        seed <<= 1;
        printf(".");
        seed_count++;
    }

    srand(seed);
}