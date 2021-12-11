#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "print.h"
#include "terminal.h"

#define BOARD_WIDTH 40
#define BOARD_HEIGHT 20
#define MAX_GENERATIONS 50

static byte current[BOARD_WIDTH][BOARD_HEIGHT];
static byte previous[BOARD_WIDTH][BOARD_HEIGHT];

static byte cycles;

void rand_prompt();

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

void rand_prompt()
{
    byte seed = 0;
    byte seed_count = 0;

    print_line("Enter some random characters: ");

    while (seed_count < 10)
    {
        seed ^= acia_getc();
        seed <<= 1;
        acia_putc('.');
        seed_count++;
    }

    srand(seed);
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
    print_newline();
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
    byte living = 0; // Running total of live neighbors for a cell
    int i, j;

    // Loop through the offsets and make sure they are valid cell positions then count live neighbors
    for (i = -1; i <= 1; i++)
    {
        for (j = -1; j <= 1; j++)
        {
            if (
                (int)x + i >= 0 &&           // Neighbor plus offset should be above zero in the x-direction
                (int)y + j >= 0 &&           // Neighbor plus offset should be above zero in the y-direction
                (int)x + i < BOARD_HEIGHT && // Neighbor plus offset should be below the height of the board
                (int)y + j < BOARD_WIDTH &&  // Neighbor plus offset should be below the width of the board
                !(i == 0 && j == 0)          // Don't consider the cell its own neighbor
            )
            {
                living += previous[x + i][y + j]; // If the neighbor is on the board, add its state to the living count
            }
        }
    }

    return living;
}
