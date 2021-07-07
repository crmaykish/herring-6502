#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <peekpoke.h>
#include "acia.h"

// #define VIA_PORTA 0x4001
// #define VIA_PORTB 0x4000
// #define VIA_DDRA 0x4003
// #define VIA_DDRB 0x4002

// static void VIA_Init()
// {
//     // Set all pins on Port B to outputs
//     POKE(VIA_DDRB, 0xFF);
// }

// static void VIA_Write(char c)
// {
//     POKE(VIA_PORTB, c);
// }

int main()
{
    // TODO: why do some of the stdlib calls fail? Is the cc65 lib missing something? Is my memory config messed up?

    unsigned addr = 0;
    unsigned char data = 0;
    char message[32] = {0};
    char *serial_buffer = ACIA_GetBuffer();

    ACIA_Init();

    ACIA_WriteLine("><(((°> Welcome to Herring 6502! ><(((°>");

    for (;;)
    {
        ACIA_Write('>');
        ACIA_Write(' ');

        ACIA_ReadLine();
        ACIA_Write('\n');

        if (serial_buffer[0] == 'r' && serial_buffer[1] == 'd')
        {
            ACIA_WriteLine("READ");

            if (serial_buffer[3] > '9')
            {
                addr += ((serial_buffer[3] - 55) << 0xC);
            }
            else
            {
                addr += ((serial_buffer[3] - '0') << 0xC);
            }

            if (serial_buffer[4] > '9')
            {
                addr += ((serial_buffer[4] - 55) << 0x8);
            }
            else
            {
                addr += ((serial_buffer[4] - '0') << 0x8);
            }

            if (serial_buffer[5] > '9')
            {
                addr += ((serial_buffer[5] - 55) << 0x4);
            }
            else
            {
                addr += ((serial_buffer[5] - '0') << 0x4);
            }

            if (serial_buffer[6] > '9')
            {
                addr += (serial_buffer[6] - 55);
            }
            else
            {
                addr += (serial_buffer[6] - '0');
            }

            data = PEEK(addr);

            // TODO: doesn't fully print with addresses < 0x1000, probably not clearing all of the nulls in the string

            utoa(addr, message, 16);
            message[4] = ':';
            message[5] = ' ';
            utoa(data, message + 6, 16);
            message[8] = 0;

            ACIA_WriteLine(message);
        }
        else if (serial_buffer[0] == 'w' && serial_buffer[1] == 'r')
        {
            ACIA_WriteLine("WRITE");
        }
        else if (serial_buffer[0] == 'j' && serial_buffer[1] == 'p')
        {
            ACIA_WriteLine("JUMP");
        }
        else
        {
            ACIA_WriteLine("Unknown command");
        }

        // Clear the buffers
        memset(serial_buffer, 0, SERIAL_BUFFER_SIZE);
        memset(message, 0, 32);
        addr = 0;
        data = 0;
    }

    return 0;
}
