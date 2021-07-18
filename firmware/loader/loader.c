#include <stdio.h>
#include "herring.h"
#include "acia.h"

#define PROGRAM_RAM_START 0x1000
#define PROGRAM_RAM_SIZE 0xB000

#define STATE_NOT_LOADING 0
#define STATE_LOADING 1
#define STATE_RUNNING 2

// TODO: there's an issue with this loader on branches newer than 2f4ed9390f2c7f9e09d84b928ae09e6f10093400
// It kind of works, but it does strange things loading sysmon and the VIA doesn't seem to work
// Probably broke something when I refactored the serial port code

extern void run_loaded_code();

int main()
{
    word program_index = 0;
    byte start_count = 0;
    byte end_count = 0;
    byte input = 0;
    byte state = STATE_NOT_LOADING;

    ACIA_Init(ACIA_TERM);

    ACIA_WriteBuffer(ACIA_TERM, "\n\rReady to load program.\n\r");

    while (1)
    {
        input = ACIA_Read(ACIA_TERM);

        if (state == STATE_NOT_LOADING)
        {
            // Watch for start pattern

            if (input == '*')
            {
                start_count++;

                if (start_count == 3)
                {
                    state = STATE_LOADING;

                    ACIA_WriteBuffer(ACIA_TERM, "Loading program...\n\r");
                }
            }
            else
            {
                start_count = 0;
            }
        }
        else if (state == STATE_LOADING)
        {
            // Store incoming data

            poke(PROGRAM_RAM_START + program_index, input);
            program_index++;

            // Watch for end pattern

            if (input == '!')
            {
                end_count++;

                if (end_count == 3)
                {
                    int i = 0;

                    state = STATE_RUNNING;

                    // Clear the last three bytes of the program (!!! back to FFs)
                    poke(PROGRAM_RAM_START + program_index - 1, 0xFF);
                    poke(PROGRAM_RAM_START + program_index - 2, 0xFF);
                    poke(PROGRAM_RAM_START + program_index - 3, 0xFF);

                    program_index -= 3;

                    ACIA_WriteBuffer(ACIA_TERM, "Loading complete.\n\r");

                    for (i; i < program_index; i++)
                    {
                        if (peek(PROGRAM_RAM_START + i) >= 32 && peek(PROGRAM_RAM_START + i) < 127)
                        {
                            ACIA_Write(ACIA_TERM, peek(PROGRAM_RAM_START + i));
                        }
                        else
                        {
                            ACIA_Write(ACIA_TERM, '.');
                        }
                    }

                    ACIA_WriteBuffer(ACIA_TERM, "\n\rPress enter to run.\n\r");
                }
            }
            else
            {
                end_count = 0;
            }
        }
        else if (state = STATE_RUNNING)
        {
            // Run the loaded program
            ACIA_WriteBuffer(ACIA_TERM, "\n\rRunning program.\n\r");
            run_loaded_code();
        }
    }

    return 0;
}
