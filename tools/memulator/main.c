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

// Emulator running flag
bool volatile ready = false;

void serial_handler()
{
    uint8_t index = 0;  // TODO: this index is overflowing and messing up the memory storage!
    uint8_t in = 0;

    uint8_t end_count = 0;

    while (true)
    {
        in = (uint8_t)getchar();

        if (ready && in == 0xDE)
        {
            index = 0;
            ready = false;
            printf("Loading...\r\n");
            gpio_put(PICO_DEFAULT_LED_PIN, false);
            continue;
        }

        if (!ready)
        {
            printf("%02X ", in);

            if (ROM_START + index <= 0xFFFF)
            {
                memory[ROM_START + index] = in;
            }

            index++;

            if (in == 0xDE)
            {
                end_count++;

                if (end_count == 3)
                {
                    ready = true;
                    printf("\r\nDONE!\r\n");
                    gpio_put(PICO_DEFAULT_LED_PIN, true);
                }
            }
            else
            {
                end_count = 0;
            }
        }
    }
}

void memory_handler()
{
    uint address = 0;

    while (true)
    {
        // Read the address bus
        address = (gpio_get_all() & LOWER_ADDR_BUS_MASK) >> 8;
        address |= (gpio_get(A15_PIN) << 15);

        // If address is in "ROM"
        if (ready && address >= ROM_START && address <= ROM_END)
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

    // Pico's built-in LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // Set interrupt vectors

    // NMI
    memory[0xFFFA] = (uint8_t)0x00;
    memory[0xFFFB] = (uint8_t)0xA0;

    // Reset
    memory[0xFFFC] = (uint8_t)0x00;
    memory[0xFFFD] = (uint8_t)0xE0;

    // IRQ
    memory[0xFFFE] = (uint8_t)0x00;
    memory[0xFFFF] = (uint8_t)0xB0;

    // Emulator ready
    ready = true;

    // Start the memory emulator on the second CPU core
    multicore_launch_core1(memory_handler);

    // Run serial handler on core 0
    serial_handler();
}
