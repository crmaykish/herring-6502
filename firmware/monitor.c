#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <peekpoke.h>

// #define VIA_PORTA 0x4001
// #define VIA_PORTB 0x4000
// #define VIA_DDRA 0x4003
// #define VIA_DDRB 0x4002

#define ACIA_DATA 0x6000
#define ACIA_STATUS 0x6001
#define ACIA_COMMAND 0x6002
#define ACIA_CONTROL 0x6003

#define SERIAL_BUFFER_SIZE 32

// Global Variables
static unsigned char serial_buffer[SERIAL_BUFFER_SIZE];
static unsigned char serial_buffer_idx = 0;

// static void VIA_Init()
// {
//     // Set all pins on Port B to outputs
//     POKE(VIA_DDRB, 0xFF);
// }

// static void VIA_Write(char c)
// {
//     POKE(VIA_PORTB, c);
// }

static void ACIA_Init()
{
    POKE(ACIA_STATUS, 0x00);
    POKE(ACIA_COMMAND, 0x0B);
    POKE(ACIA_CONTROL, 0x1E);
}

static void ACIA_Write(char c)
{
    while ((PEEK(ACIA_STATUS) & 0x10) == 0)
    {
        // Wait for ACIA Tx ready flag
    }

    // Write the character to the ACIA
    POKE(ACIA_DATA, c);
}

static void ACIA_NewLine()
{
    ACIA_Write('\n');
    ACIA_Write('\r');
}

static void ACIA_WriteBuffer(char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        ACIA_Write(buffer[i]);
        i++;
    }
}

static void ACIA_WriteLine(char *message)
{
    ACIA_WriteBuffer(message);
    ACIA_NewLine();
}

static unsigned char ACIA_Read()
{
    while ((PEEK(ACIA_STATUS) & 0x08) == 0)
    {
        // Wait for ACIA Rx data
    }

    return PEEK(ACIA_DATA);
}

static void ACIA_ReadLine()
{
    for (;;)
    {
        serial_buffer[serial_buffer_idx] = ACIA_Read();

        // echo back to the console
        ACIA_Write(serial_buffer[serial_buffer_idx]);

        if (serial_buffer[serial_buffer_idx] == '\n' || serial_buffer[serial_buffer_idx] == '\r')
        {
            serial_buffer_idx = 0;
            return;
        }

        serial_buffer_idx++;

        if (serial_buffer_idx == SERIAL_BUFFER_SIZE)
        {
            serial_buffer_idx = 0;
            return;
        }
    }
}

int main()
{
    unsigned addr = 0;
    unsigned char data = 0;
    char message[32] = {0};

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
