#include "acia.h"

void ACIA_Init()
{
    // Reset ACIA
    poke(ACIA1_STATUS, 0x00);
    // // RTS low, transmit interupt disabled, receiver interupt disabled, DTR low
    poke(ACIA1_COMMAND, 0x0B); 
    // Set baudrate to 19200, 8 data bits, 1 stop bits
    poke(ACIA1_CONTROL, 0b00011111);
}

byte ACIA_Read()
{
    while ((peek(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return peek(ACIA1_DATA);
}

byte ACIA_ReadLine(char *buffer, byte max, bool echo)
{
    byte bytes_read = 0;
    byte in = 0;

    while (in != '\r' && bytes_read < max)
    {
        in = ACIA_Read();
        
        if (echo)
        {
            ACIA_Write(in);
        }

        buffer[bytes_read] = in;
        bytes_read++;
    }

    // NULL-terminate the buffer
    buffer[bytes_read] = 0;

    return bytes_read;
}

void ACIA_Write(char c)
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
    ACIA_Write(ASCII_NEWLINE);
    ACIA_Write(ASCII_CARRIAGE_RETURN);
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
