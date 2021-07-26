#include "acia.h"
#include "herring.h"

#define PROGRAM_START 0x400 // TODO: define this from the linker RAM values?

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

    ACIA_Init();

    ACIA_WriteBuffer("\r\nHerring 6502 Serial Loader v2.0\r\n");
    ACIA_WriteBuffer("Ready to load program.\r\n");

    while (true)
    {
        // Wait for a byte from the serial port
        in = ACIA_Read();

        switch (program_state)
        {
        case READY:
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

            break;
        case LOADING:

            // Too slow to do this in line?
            if (in >= 32 && in < 127)
            {
                ACIA_Write(in);
            }
            else
            {
                ACIA_Write('.');
            }

            // Watch for end bytes
            if (in == '!')
            {
                end_byte_count++;

                if (end_byte_count == 3)
                {
                    // Done loading program code
                    ACIA_WriteBuffer("\r\nLoading complete. Press enter to run.\r\n");
                    program_state = RUNNING;
                }
            }
            else
            {
                end_byte_count = 0;
            }

            break;
        case RUNNING:

            asm("jmp %w", PROGRAM_START);

            break;

        default:
            break;
        }
    }

    return 0;
}
