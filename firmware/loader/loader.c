#include "herring.h"
#include <stdio.h>

#define PROGRAM_RAM_START 0x1000
#define PROGRAM_RAM_SIZE 0xB000

extern void run_loaded_code();

byte ACIA_Read()
{
    while ((peek(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return peek(ACIA1_DATA);
}

void ACIA_Write(char c)
{
    while ((peek(ACIA1_STATUS) & 0x10) == 0)
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

void ACIA_WriteBuffer(char *buffer)
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
    byte loading = 0;
    byte running = 0;

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

    ACIA_WriteBuffer("Waiting for input...\n\r");

    // TODO: This is actually working, but * is valid data and it's getting ignored in the loading

    while (1)
    {
        input = ACIA_Read();

        if (input == '*')
        {
            start_count++;
        }
        else if (input == '!')
        {
            end_count++;
        }
        else
        {
            start_count = 0;
            end_count = 0;
        }

        if (start_count == 3)
        {
            ACIA_WriteBuffer("Loading program...\n\r");
            loading = 1;
            running = 0;
            start_count = 0;
        }

        if (end_count == 3)
        {
            loading = 0;
            running = 1;
            start_count = 0;
            end_count = 0;
        }

        if (loading && input != '*' && input != '!')
        {
            // ACIA_Write(input);
            // sprintf(buffer, "%d %02X\n\r", i, input);
            // ACIA_WriteBuffer(buffer);

            poke(PROGRAM_RAM_START + program_index, input);

            program_index++;
        }

        else if (running)
        {
            int i = 0;
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
            ACIA_WriteBuffer("\n\rRunning program!\n\r");
            run_loaded_code();
        }
    }

    return 0;
}
