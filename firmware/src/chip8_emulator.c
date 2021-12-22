#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "serial.h"
#include "chip8.h"

#define LOCAL_ROM_ADDR (0x7000)

bool running = true;

void draw_p(uint8_t x, uint8_t y, chip8_pixel_state_e p)
{
    term_cursor_set_pos(x + x, y);

    if (p == CHIP8_PIXEL_ON)
    {
        term_set_color(TERM_BG_BRIGHT_GREEN);
    }
    else
    {
        term_set_color(TERM_RESET);
    }

    putc(' ');
    putc(' ');
}

void poll_input()
{
    uint8_t input = 0xFF;

    // Check for serial input
    if (serial_byte_available())
    {
        input = getc();

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
    chip8_set_clear_screen_func(&term_clear);
    // TODO: set the random byte function

    // Load the CHIP-8 ROM file into the emulator's system memory from local memory
    status = chip8_load_rom((uint8_t *)LOCAL_ROM_ADDR, CHIP8_ROM_MAX_SIZE);

    if (status != CHIP8_SUCCESS)
    {
        puts("Error loading ROM, Status: ");
        print_hex(status);
        return 0;
    }

    term_clear();
    term_cursor_set_vis(false);

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

        // TODO: Timers don't tick at a constant rate, use an actual timer from the VIA
        chip8_tick_timers();
    }

    term_set_color(TERM_RESET);
    term_clear();
    term_cursor_set_vis(true);

    puts("Emulator stopped with state = ");
    print_dec(run_state);
    puts(", status = ");
    print_dec(status);
    return 0;
}
