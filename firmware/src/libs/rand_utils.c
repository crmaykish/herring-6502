#include <stdlib.h>
#include "rand_utils.h"
#include "acia.h"
#include "print.h"

void rand_prompt()
{
    uint8_t seed;
    uint8_t seed_count = 0;

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

uint8_t rand_byte()
{
    return (uint8_t)(rand() % 0xFF);
}
