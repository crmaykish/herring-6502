#include "spi.h"
#include "acia.h"

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

void SD_Init()
{
    byte spi_data = 0;
    byte sd_init = 0;

    SPI_Init();
    spi_data = SPI_ReadByte();
    poke(0xC401, 0b00001100);

    print("Warmup Byte: ");
    print_hex(spi_data);
    print("\r\n");

    spi_data = SPI_SendCommand(SPI_COMMAND_0);

    print("CMD_0 Byte: ");
    print_hex(spi_data);
    print("\r\n");

    spi_data = SPI_SendCommand(SPI_COMMAND_8);

    // ignore the 32-bit response
    SPI_ReadByte();
    SPI_ReadByte();
    SPI_ReadByte();
    SPI_ReadByte();

    print("CMD_8 Byte: ");
    print_hex(spi_data);
    print("\r\n");

    while (sd_init == 0)
    {
        spi_data = SPI_SendCommand(SPI_COMMAND_55);

        print("CMD_55 Byte: ");
        print_hex(spi_data);
        print("\r\n");

        spi_data = SPI_SendCommand(SPI_COMMAND_41);

        print("CMD_41 Byte: ");
        print_hex(spi_data);
        print("\r\n");

        if (spi_data == 0)
        {
            sd_init = 1;
        }
    }

    print("SD card initialized.\r\n");
}

void SD_ReadBlock(byte *block, byte b3, byte b2, byte b1, byte b0)
{
    byte spi_data = 0;
    word spi_index = 0;

    byte command[6];

    command[0] = SPI_COMMAND_51;
    command[1] = b3;
    command[2] = b2;
    command[3] = b1;
    command[4] = b0;
    command[5] = 0x01;

    spi_data = SPI_SendCommand(command);

    if (spi_data == 0)
    {
        print("Initialize block read...\r\n");
    }
    else
    {
        print("Error initializing read.\r\n");
        return;
    }

    while (spi_data != 0xFE)
    {
        spi_data = SPI_ReadByte();
    }

    print("Start reading block data\r\n");


    // TODO: getting stuck in here. Reading some 0s sometimes, probably a clock issue? Move it all to ASM to clarity

    for (spi_index = 0; spi_index < 512; spi_index++)
    {
        spi_data = SPI_ReadByte();
        block[spi_index] = spi_data;
        print_hex(spi_data);
        put(' ');
    }

    // disable SPI chip select
    poke(0xC401, 0b000000101);
}
