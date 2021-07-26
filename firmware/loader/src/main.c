#include "via.h"
#include "acia.h"
#include "herring.h"

#define PROGRAM_START 0x1000 // TODO: define this from the linker RAM values?

typedef enum
{
    READY,
    LOADING,
    RUNNING
} State_e;

int main()
{
    // Current state of the loader program
    State_e program_state = READY;
    // Serial input byte
    byte in = 0;
    // Number of bytes stored into program memory
    word in_count = 0;

    byte start_byte_count = 0;
    byte end_byte_count = 0;

    word i = 0;

    VIA_OutputAll();
    VIA_SetPortA(0);
    VIA_SetPortB(0);

    ACIA_Init();

    ACIA_WriteBuffer("\r\nHerring 6502 Serial Loader v2.0\r\n");
    ACIA_WriteBuffer("Ready to load program.\r\n");

    while (true)
    {
        // Wait for a byte from the serial port
        in = ACIA_Read();

        if (in >= 32 && in < 127)
        {
            ACIA_Write(in);
        }
        else
        {
            ACIA_Write('.');
        }

        if (program_state == READY)
        {
            // Watch for start bytes
            if (in == '*')
            {
                start_byte_count++;

                if (start_byte_count == 3)
                {
                    // Got the start bytes, ready to load program code into RAM
                    ACIA_WriteBuffer("Loading...\r\n");
                    program_state = LOADING;
                }
            }
            else
            {
                start_byte_count = 0;
            }
        }
        else if (program_state == LOADING)
        {

            // Store byte into RAM
            // POKE(PROGRAM_START + in_count, in);
            in_count++;

            VIA_SetPortA(in_count & 0xFF);
            VIA_SetPortB((in_count & 0xFF00) >> 8);

            // Watch for end bytes
            if (in == '!')
            {
                ACIA_WriteBuffer("end");

                end_byte_count++;

                if (end_byte_count == 3)
                {
                    // Done loading program code
                    // ACIA_WriteBuffer("\r\n");

                    // for (i; i < in_count; i++)
                    // {
                    //     if (PEEK(PROGRAM_START + i) >= 32 && PEEK(PROGRAM_START + i) < 127)
                    //     {
                    //         ACIA_Write(PEEK(PROGRAM_START + i));
                    //     }
                    //     else
                    //     {
                    //         ACIA_Write('.');
                    //     }
                    // }

                    ACIA_WriteBuffer("\r\nLoading complete. Press enter to run.\r\n");
                    program_state = RUNNING;
                }
            }
            else
            {
                end_byte_count = 0;
            }
        }
        else if (program_state == RUNNING)
        {
            ACIA_WriteBuffer("Starting program...\r\n");
            asm("jmp %w", PROGRAM_START);
            break;
        }
    }

    return 0;
}
