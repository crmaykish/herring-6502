#include "herring.h"
#include <stdio.h>

#define PROGRAM_RAM_START 0x1000
#define PROGRAM_RAM_SIZE 0xB000

#define STATE_NOT_LOADING 0
#define STATE_LOADING 1
#define STATE_RUNNING 2

extern void run_loaded_code();

byte ACIA_Read()
{
    while ((peek(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return peek(ACIA1_DATA);
}

void __fastcall__ ACIA_Write(char c)
{
    while ((peek(ACIA1_STATUS) & ACIA_READY_TX) == 0)
    {
        // Wait for ACIA Tx ready flag
    }

    // Write the character to the ACIA
    poke(ACIA1_DATA, c);
}

void ACIA_NewLine()
{
    ACIA_Write('\n');
    ACIA_Write('\r');
}

void __fastcall__ ACIA_WriteBuffer(char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        ACIA_Write(buffer[i]);
        i++;
    }
}

int main()
{
    word program_index = 0;
    byte start_count = 0;
    byte end_count = 0;
    byte input = 0;
    byte state = STATE_NOT_LOADING;

    // Init VIA
    poke(VIA1_DDRA, 0xFF);
    poke(VIA1_DDRB, 0xFF);
    // Clear VIA ports
    poke(VIA1_PORTA, 0x00);
    poke(VIA1_PORTB, 0x00);

    // Init ACIA
    poke(ACIA1_STATUS, 0x00);
    poke(ACIA1_COMMAND, 0x0B);
    poke(ACIA1_CONTROL, 0x1E);

    ACIA_WriteBuffer("\n\rWaiting for input...\n\r");

    while (1)
    {
        input = ACIA_Read();

        if (state == STATE_NOT_LOADING)
        {
            // Watch for start pattern

            if (input == '*')
            {
                start_count++;

                if (start_count == 3)
                {
                    state = STATE_LOADING;

                    ACIA_WriteBuffer("Loading program...\n\r");
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

                    ACIA_WriteBuffer("Loading complete.\n\r");

                    for (i; i < program_index; i++)
                    {
                        if (peek(PROGRAM_RAM_START + i) >= 32 && peek(PROGRAM_RAM_START + i) < 127)
                        {
                            ACIA_Write(peek(PROGRAM_RAM_START + i));
                        }
                        else
                        {
                            ACIA_Write('.');
                        }
                    }

                    ACIA_WriteBuffer("\n\rPress enter to run.\n\r");
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
            ACIA_WriteBuffer("\n\rRunning program.\n\r");
            run_loaded_code();
        }
    }

    return 0;
}
