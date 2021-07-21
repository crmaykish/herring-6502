#include "spi.h"

byte SPI_SendCommand(byte *command)
{
    byte b = 0;
    byte i = 0;

    // toggle the clock a bunch of times with CS high
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);
    poke(0xC401, 0b00001101);
    poke(0xC401, 0b00001100);

    // pull CS low
    poke(0xC401, 0b00000100);

    for (i; i < SPI_COMMAND_LENGTH; i++)
    {
        SPI_WriteByte(command[i]);
    }

    b = SPI_WaitResult();

    // pull CS high
    poke(0xC401, 0b00001100);

    return b;
}
