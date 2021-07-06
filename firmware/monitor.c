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

// TODO: fast calls

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

static void ACIA_WriteLine(char *message)
{
    unsigned char i = 0;
    while (message[i] != 0)
    {
        ACIA_Write(message[i]);
        i++;
    }

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
    unsigned char i = 0;

    ACIA_Init();

    ACIA_WriteLine("><(((°> Welcome to Herring 6502! ><(((°>");

    for (;;)
    {
        ACIA_Write('>');
        ACIA_Write(' ');

        ACIA_ReadLine();

        ACIA_Write('\n');
        ACIA_WriteLine(serial_buffer);

        // TODO: strncmp is blowing up
        // if (strncmp(serial_buffer, "peek", 4))
        // {
        //     ACIA_WriteLine("PEEKING");
        // }
        // else if (strncmp(serial_buffer, "poke", 4))
        // {
        //     ACIA_WriteLine("POKING");
        // }

        // Clear the serial buffer
        memset(serial_buffer, 0, SERIAL_BUFFER_SIZE);

        i = 0;
    }

    return 0;
}
