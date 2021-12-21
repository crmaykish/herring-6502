#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "print.h"
#include "terminal.h"
#include "rand_utils.h"

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 22
#define MAX_GENERATIONS 50

static byte current[BOARD_WIDTH][BOARD_HEIGHT];
static byte previous[BOARD_WIDTH][BOARD_HEIGHT];

static byte cycles;
static byte seed;

void init_board();
void draw_board();
void update_state();

byte count_neighbors(byte x, byte y);

int main()
{
    rand_prompt();

    set_cursor_visible(false);

    screen_clear();

    init_board();

    while (cycles <= MAX_GENERATIONS)
    {
        update_state();
        draw_board();
        cycles++;
    }

    set_cursor_visible(true);

    return 0;
}

void init_board()
{
    byte i, j;

    cycles = 1;

    for (j = 0; j < BOARD_HEIGHT; j++)
    {
        for (i = 0; i < BOARD_WIDTH; i++)
        {
            previous[i][j] = ((rand() % 10) < 3) ? 1 : 0;
        }
    }
}

void draw_board()
{
    byte i, j;

    cursor_set_pos(0, 0);

    term_set_color(BG_COLOR_MAGENTA);
    print_line("Conway's Game of Life");
    term_set_color(DEFAULT_COLORS);

    for (j = 0; j < BOARD_HEIGHT; j++)
    {
        for (i = 0; i < BOARD_WIDTH; i++)
        {
            if (current[i][j] == 1)
            {
                term_set_color(BG_COLOR_BRIGHT_GREEN);
                print("  ");
            }
            else
            {
                term_set_color(DEFAULT_COLORS);
                print("  ");
            }
        }

        print_newline();
    }

    term_set_color(BG_COLOR_MAGENTA);
    print("Generations: ");
    print_dec(cycles);
    print("           Seed: ");
    print_dec(seed);
    term_set_color(DEFAULT_COLORS);
}

void update_state()
{
    byte i, j;
    byte neighbors;

    // Clear current board
    memset(current, 0, BOARD_WIDTH * BOARD_HEIGHT);

    for (j = 0; j < BOARD_HEIGHT; j++)
    {
        for (i = 0; i < BOARD_WIDTH; i++)
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

byte count_neighbors(byte x, byte y)
{
    // Living neighbors
    byte living = 0;

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
