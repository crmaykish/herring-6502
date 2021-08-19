#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/time.h"

#define RW_MASK (1 << 27)
#define A15_MASK (1 << 26)
#define LOWER_ADDR_BUS_MASK 0b11111111111111100000000
#define ADDR_BUS_MASK (A15_MASK | LOWER_ADDR_BUS_MASK)
#define DATA_BUS_MASK 0xFF

#define ALL_PINS (DATA_BUS_MASK | ADDR_BUS_MASK | RW_MASK)

#define CLK_PIN 28

// 64K memory block
static uint8_t memory[0xFFFF] = {0};

uint reverse_bits(uint b)
{
    return ((b & 0b1) << 7) |
           ((b & 0b10) << 5) |
           ((b & 0b100) << 3) |
           ((b & 0b1000) << 1) |
           ((b & 0b10000) >> 1) |
           ((b & 0b100000) >> 3) |
           ((b & 0b1000000) >> 5) |
           ((b & 0b10000000) >> 7);
}

int main()
{
    stdio_init_all();

    // Set up the GPIO pins
    gpio_init_mask(ALL_PINS);
    gpio_set_dir_in_masked(ALL_PINS);

    gpio_init(CLK_PIN);
    gpio_set_dir(CLK_PIN, GPIO_OUT);

    // TODO: store the ROM image in Flash and copy it into RAM on boot
    // TODO: make ROM/IO memory areas readonly

    // Write NOPs to all memory locations
    memset(memory, 0xEA, 0xFFFF);

    // Store the program code to "ROM"
    uint8_t code[] = {0xA9, 0x00, 0x8D, 0x80, 0x00, 0x69, 0x01, 0x4C, 0x02, 0x80};
    memcpy(&memory[0x8000], code, sizeof(code) / sizeof(uint8_t));

    // Set interrupt vectors
    memory[0xFFFA] = (uint8_t)0x00;
    memory[0xFFFB] = (uint8_t)0xA0;
    memory[0xFFFC] = (uint8_t)0x00;
    memory[0xFFFD] = (uint8_t)0x80;
    memory[0xFFFE] = (uint8_t)0x00;
    memory[0xFFFF] = (uint8_t)0xB0;

    uint address = 0;
    bool rw = true; // read = true, write = false

    bool cpu_state = false;

    absolute_time_t last_time = {0};

    printf("Starting...\r\n");

    while (true)
    {
        cpu_state = !cpu_state;
        gpio_put(CLK_PIN, cpu_state);

        if (cpu_state)
        {
            // Read the address bus and the read/write pin
            address = ((gpio_get_all() & LOWER_ADDR_BUS_MASK) >> 8) | gpio_get(26) << 15;
            rw = gpio_get(27);

            if (address < 0xC000 || address >= 0xE000)
            {
                // Address is in memory
                if (rw)
                {
                    // Read from RAM

                    // Output memory value to the data bus
                    gpio_set_dir_out_masked(DATA_BUS_MASK);
                    sleep_ms(1);
                    gpio_put_masked(DATA_BUS_MASK, reverse_bits(memory[address]));

                    // printf("R: %04X %02X\r\n", address, memory[address]);
                }
                else
                {
                    // Write to RAM

                    // Store data bus into memory
                    gpio_set_dir_in_masked(DATA_BUS_MASK);
                    sleep_ms(1);    // Pico writes to the databus too quickly when CPU changes
                    // This might not be an issue with an external clock
                    memory[address] = reverse_bits((gpio_get_all() & DATA_BUS_MASK));

                    // printf("W: %04X %02X\r\n", address, memory[address]);
                }
            }
            else
            {
                // Address is in the I/O space
                gpio_set_dir_in_masked(DATA_BUS_MASK);
                
            }
            sleep_ms(10);
        }
        sleep_ms(25);
    }
}
