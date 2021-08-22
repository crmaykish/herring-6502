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

#define IO_START 0xF800
#define IO_END 0xFC00

// 64K memory block
static uint8_t memory[0xFFFF] = {0};

// Program code
static uint8_t code[] = {0xa9, 0x00, 0x8d, 0x01, 0xf8, 0xa9, 0x0b, 0x8d, 0x02, 0xf8, 0xa9, 0x1f, 0x8d, 0x03, 0xf8, 0xa2, 0x20, 0x8e, 0x00, 0xf8, 0xe0, 0x7e, 0xf0, 0xf7, 0xe8, 0x4c, 0x11, 0x02};

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
    bool rw = true; // read = true, write = false

    // Turn the LED on when emulation starts
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    while (true)
    {
        // Read the address bus and the read/write pin
        address = (gpio_get_all() & LOWER_ADDR_BUS_MASK) >> 8;
        address |= (gpio_get(A15_PIN) << 15);

        rw = gpio_get(RW_PIN);

        // If address is not in the I/O space
        if (address < IO_START || address >= IO_END)
        {
            if (rw)
            {
                // Output memory value to the data bus
                gpio_set_dir_out_masked(DATA_BUS_MASK);
                gpio_put_masked(DATA_BUS_MASK, memory[address]);
            }
            else
            {
                // Store data bus into memory
                gpio_set_dir_in_masked(DATA_BUS_MASK);
                memory[address] = (gpio_get_all() & DATA_BUS_MASK);
            }
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
    memset(memory, 0xEA, 0xFFFF);

    // Store the program code to "ROM"
    memcpy(&memory[0x200], code, sizeof(code) / sizeof(uint8_t));

    // Set interrupt vectors
    memory[0xFFFA] = (uint8_t)0x00;
    memory[0xFFFB] = (uint8_t)0xA0;
    memory[0xFFFC] = (uint8_t)0x00;
    memory[0xFFFD] = (uint8_t)0x02;
    memory[0xFFFE] = (uint8_t)0x00;
    memory[0xFFFF] = (uint8_t)0xB0;

    // Start the memory emulator on the second CPU core
    multicore_launch_core1(memory_handler);

    // Run serial handler on core 0
    serial_handler();
}
