#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"

#define A15_PIN 26
#define RW_PIN 27
#define BE_PIN 28

#define BE_MASK (1 << BE_PIN)
#define RW_MASK (1 << RW_PIN)
#define A15_MASK (1 << A15_PIN)

#define LOWER_ADDR_BUS_MASK 0b11111111111111100000000
#define ADDR_BUS_MASK (A15_MASK | LOWER_ADDR_BUS_MASK)
#define DATA_BUS_MASK 0xFF
#define ALL_PINS (DATA_BUS_MASK | ADDR_BUS_MASK | RW_MASK)

#define ROM_START 0xE000
#define ROM_END 0xFFFF

// 64K memory block
static uint8_t memory[0xFFFF] = {0};

// Program code
static uint8_t code[] = {0xa9, 0x00, 0x8d, 0x01, 0x80, 0xa9, 0x0b, 0x8d, 0x02, 0x80, 0xa9, 0x1f, 0x8d, 0x03, 0x80, 0xa9, 0xff, 0x8d, 0x02, 0x84, 0xa9, 0x00, 0x8d, 0x00, 0x84, 0xa2, 0x20, 0x8e, 0x00, 0x80, 0x8e, 0x00, 0x84, 0xe0, 0x7e, 0xf0, 0xf4, 0xe8, 0x4c, 0x1b, 0xe0};

void serial_handler()
{
    uint8_t in = 0;

    while (true)
    {
        in = (uint8_t)getchar();
        putchar(in);
    }
}

void memory_handler()
{
    uint address = 0;

    // Turn the LED on when emulation starts
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    while (true)
    {
        // Read the address bus
        address = (gpio_get_all() & LOWER_ADDR_BUS_MASK) >> 8;
        address |= (gpio_get(A15_PIN) << 15);

        // If address is in "ROM"
        if (address >= ROM_START && address <= ROM_END)
        {
            // Output memory value to the data bus
            gpio_set_dir_out_masked(DATA_BUS_MASK);
            gpio_put_masked(DATA_BUS_MASK, memory[address]);
        }
        else
        {
            // Set data bus to inputs (high impedance)
            gpio_set_dir_in_masked(DATA_BUS_MASK);
        }
    }
}

int main()
{
    stdio_init_all();

    // Set up the GPIO pins
    gpio_init_mask(ALL_PINS);
    gpio_set_dir_in_masked(ALL_PINS);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // Write NOPs to all memory locations
    // memset(memory, 0xEA, 0xFFFF);

    // Store the program code to "ROM"
    memcpy(&memory[ROM_START], code, sizeof(code) / sizeof(uint8_t));

    // Set interrupt vectors
    memory[0xFFFA] = (uint8_t)0x00;
    memory[0xFFFB] = (uint8_t)0xA0;
    memory[0xFFFC] = (uint8_t)0x00;
    memory[0xFFFD] = (uint8_t)0xE0;
    memory[0xFFFE] = (uint8_t)0x00;
    memory[0xFFFF] = (uint8_t)0xB0;

    // Start the memory emulator on the second CPU core
    multicore_launch_core1(memory_handler);

    // Run serial handler on core 0
    serial_handler();
}
