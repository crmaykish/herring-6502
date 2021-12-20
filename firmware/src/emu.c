#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"
#include "rand_utils.h"

#include "chip8.h"

#define LOCAL_ROM_ADDR (0x7000)

bool running = true;

void draw_p(uint8_t x, uint8_t y, chip8_pixel_state_e p)
{
    cursor_set_pos(y, x + x);
    acia_putc(p == CHIP8_PIXEL_ON ? '#' : ' ');
}

void draw_screen(bool clear)
{
    if (clear)
    {
        screen_clear();
    }
}

void poll_input()
{
    uint8_t input = 0xFF;

    // Check for serial input
    if (acia_rx_ready())
    {
        input = acia_getc();

        switch (input)
        {
        case 0x1B: // ESC
            input = 0xFF;
            running = false;
            break;
        case 0x30: // 0
        case 0x31: // 1
        case 0x32: // 2
        case 0x33: // 3
        case 0x34: // 4
        case 0x35: // 5
        case 0x36: // 6
        case 0x37: // 7
        case 0x38: // 8
        case 0x39: // 9
            input -= 0x30;
            break;
        default:
            break;
        }
    }

    // TODO: A-F keys

    if (input != 0xFF)
    {
        chip8_press_key(input);
    }
}

int main()
{
    chip8_status_e status;
    chip8_run_state_e run_state;

    // Initialize the CHIP-8 emulator
    chip8_init();
    chip8_set_set_pixel_func(&draw_p);
    chip8_set_redraw_screen_func(&draw_screen);
    // TODO: set the random byte function

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

    running = true;

    while (running)
    {
        run_state = chip8_get_run_state();

        switch (run_state)
        {
        case CHIP8_STATE_RUNNING:
            poll_input();
            status = chip8_cycle();
            break;
        case CHIP8_STATE_WAIT_FOR_INPUT:
            poll_input();
            break;
        default:
            running = false;
            break;
        }

        chip8_tick_timers();
    }

    term_set_color(DEFAULT_COLORS);
    screen_clear();
    set_cursor_visible(true);

    print("Emulator stopped with state = ");
    print_dec(run_state);
    print(", status = ");
    print_dec(status);
    return 0;
}
