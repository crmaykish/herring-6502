#include "herring.h"

#define PROGRAM_RAM_START 0x1000
#define PROGRAM_RAM_SIZE 0xB000

extern void run_loaded_code();

// VIA shift test program
byte test_program[] = {0xA9, 0xFF, 0x8D, 0x02, 0xC4, 0xA9, 0x55, 0x8D, 0x00, 0xC4, 0x2A, 0x4C, 0x07, 0x10};

int main()
{
    int i;

    // Load the test program into RAM
    for (i = 0; i < sizeof(test_program); i++)
    {
        poke(PROGRAM_RAM_START + i, test_program[i]);
    }

    poke(VIA1_DDRA, 0xFF);
    poke(VIA1_DDRB, 0xFF);
    poke(VIA1_PORTA, 0x00);
    poke(VIA1_PORTB, 0x00);

    // Jump to the loaded code
    run_loaded_code();

    while (1)
    {
    }

    return 0;
}
