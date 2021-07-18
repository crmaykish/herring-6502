#include "acia.h"

void ACIA_Init(byte acia)
{
    // Reset ACIA
    poke(ACIA_STATUS(acia), 0x00);
    // RTS low, transmit interupt disabled, receiver interupt disabled, DTR low
    poke(ACIA_COMMAND(acia), 0x0B);
    // Set baudrate to 19200, 8 data bits, 1 stop bits
    poke(ACIA_CONTROL(acia), 0b00011111);
}

byte ACIA_Read(byte acia)
{
    while ((peek(ACIA_STATUS(acia)) & ACIA_READY_RX) == 0)
    {
    }

    return peek(ACIA_DATA(acia));
}

byte ACIA_ReadLine(byte acia, char *buffer, byte max, bool echo)
{
    byte bytes_read = 0;
    byte in = 0;

    while (in != 0x0A && in != 0x0D && bytes_read < max)
    {
        in = ACIA_Read(acia);

        if (echo)
        {
            ACIA_Write(acia, in);
        }

        buffer[bytes_read] = in;
        bytes_read++;
    }

    // NULL-terminate the buffer
    buffer[bytes_read] = 0;

    return bytes_read;
}

void ACIA_Write(byte acia, char c)
{
    while ((peek(ACIA_STATUS(acia)) & ACIA_READY_TX) == 0)
    {
        // Wait for ACIA Tx ready flag
    }

    // Write the character to the ACIA
    poke(ACIA_DATA(acia), c);
}

void ACIA_NewLine(byte acia)
{
    ACIA_Write(acia, ASCII_NEWLINE);
    ACIA_Write(acia, ASCII_CARRIAGE_RETURN);
}

void ACIA_WriteBuffer(byte acia, char *buffer)
{
    unsigned char i = 0;
    while (buffer[i] != 0)
    {
        ACIA_Write(acia, buffer[i]);
        i++;
    }
}
