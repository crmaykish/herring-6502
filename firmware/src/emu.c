#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"
#include "rand_utils.h"

#include "chip8.h"

#define PIXEL_ON '#'
#define PIXEL_OFF ' '

#define LOCAL_ROM_ADDR (0x7000)

static uint8_t pixel;

void draw_byte(uint8_t p, uint8_t x, uint8_t y)
{
    // TODO: Not XORing the pixels on
    // TODO: Not handling wrapping pixels around the screen

    byte i;

    cursor_set_pos(y, x + x);

    for (i = 0; i < 8; i++)
    {

        if ((x + i) < CHIP8_SCREEN_WIDTH)
        {
            pixel = (p & (1 << (7 - i))) > 0 ? PIXEL_ON : PIXEL_OFF;

            // Draw two characters to get the pixel proportions right
            acia_putc(pixel);
            acia_putc(pixel);
        }
    }
}

int main()
{
    chip8_status_e status;
    uint8_t input;

    rand_prompt();

    // Initialize the CHIP-8 emulator
    chip8_init(&rand_byte, &draw_byte);

    // Load the CHIP-8 ROM file into the emulator's system memory from local memory
    status = chip8_load_rom((uint8_t *)LOCAL_ROM_ADDR, CHIP8_ROM_MAX_SIZE);

    if (status != CHIP8_SUCCESS)
    {
        print("Error loading ROM, Status: ");
        print_hex(status);
        return 0;
    }

    screen_clear();
    set_cursor_visible(false);

    while (status == CHIP8_SUCCESS)
    {
        // Check for serial input
        if (acia_rx_ready())
        {
            input = acia_getc();

            if (input == 0x1B)
            {
                // If ESC was pressed, stop the emulator
                break;
            }
        }

        status = chip8_cycle();
    }

    term_set_color(DEFAULT_COLORS);
    screen_clear();
    set_cursor_visible(true);

    print("Emulator stopped with status: ");
    print_dec(status);
    return 0;
}
