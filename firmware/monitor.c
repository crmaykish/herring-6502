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
            addr = (unsigned)strtol(serial_buffer + 2, NULL, 16);
            data = PEEK(addr);

            // Note: sprintf is pretty slow
            sprintf(message, "%04X: %02X\n", addr, data);

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
