#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "via.h"
#include "spi.h"
#include "utils.h"

// Note: global variables mess with the reset vector and everything breaks...

void memdump(word addr, byte rows)
{
    byte data;
    char data_s[3];
    char addr_s[6];
    word index = 0;
    byte i = 0;
    byte j = 0;

    if (rows == 0)
    {
        rows = 8;
    }

    for (j; j < rows; j++)
    {
        itoa(addr + index, addr_s, 16);

        ACIA_WriteBuffer(ACIA_TERM, addr_s);
        ACIA_WriteBuffer(ACIA_TERM, "  ");

        for (i = 0; i < 16; i++)
        {
            data = peek(addr + index);
            itoa(data, data_s, 16);

            if (data < 0x10)
            {
                ACIA_Write(ACIA_TERM, '0');
            }

            ACIA_WriteBuffer(ACIA_TERM, data_s);

            if (i == 7)
            {
                ACIA_Write(ACIA_TERM, ' ');
            }

            ACIA_Write(ACIA_TERM, ' ');

            index++;
        }

        index -= 16;

        // Write the ASCII characters
        ACIA_WriteBuffer(ACIA_TERM, "  |");
        for (i = 0; i < 16; i++)
        {
            data = peek(addr + index);

            if (data >= 32 && data < 127)
            {
                ACIA_Write(ACIA_TERM, data);
            }
            else
            {
                ACIA_Write(ACIA_TERM, '.');
            }

            index++;
        }

        ACIA_Write(ACIA_TERM, '|');

        ACIA_NewLine(ACIA_TERM);
    }
}

void ParseCommand(char *buffer)
{
    word addr;
    byte data;

    // byte addr_s[5];
    byte data_s[3];

    char *firstParam;
    char *secondParam;

    // Isolate the parameters
    firstParam = strchr(buffer, ' ') + 1; // Note: first param includes the second param too
    secondParam = strchr(firstParam, ' ') + 1;

    if (strncmp(buffer, "memdump", 7) == 0)
    {
        addr = IntegerValue(firstParam);
        data = IntegerValue(secondParam);
        memdump(addr, data);
    }
    else if (strncmp(buffer, "peek", 4) == 0)
    {
        addr = IntegerValue(firstParam);
        itoa(peek(addr), data_s, 16);
        ACIA_WriteBuffer(ACIA_TERM, data_s);
    }
    else if (strncmp(buffer, "poke", 4) == 0)
    {
        addr = IntegerValue(firstParam);
        data = IntegerValue(secondParam);
        poke(addr, data);
    }
    else if (strncmp(buffer, "reset", 5) == 0)
    {
        // Soft reset the system monitor
        asm("jmp %w", PROGRAM_RAM);
    }
    else if (strncmp(buffer, "load", 4) == 0)
    {
        // Reset to the ROM loader program
        asm("jmp %w", ROM_PROGRAM);
    }
    else if (strncmp(buffer, "cls", 3) == 0)
    {
        byte i = 0;

        for (i; i < 80; i++)
        {
            ACIA_NewLine(ACIA_TERM);
        }
    }
    else if (strncmp(buffer, "free", 4) == 0)
    {
        size_t free_ram = _heapmaxavail();
        utoa(free_ram, buffer, 10);

        ACIA_WriteBuffer(ACIA_TERM, "Free RAM: ");
        ACIA_WriteBuffer(ACIA_TERM, buffer);
        ACIA_WriteBuffer(ACIA_TERM, " bytes");
    }
}

int main()
{
    char buffer[40];
    byte spi_data = 0;
    byte i = 0;
    byte sd_init = 0;
    word spi_index = 0;
    byte *spi_block;
    size_t free_ram;

    ACIA_Init(ACIA_TERM);

    ACIA_WriteBuffer(ACIA_TERM, "############################################################\r\n");
    ACIA_WriteBuffer(ACIA_TERM, " _    _                _                 __ _____  ___ ___  \r\n");
    ACIA_WriteBuffer(ACIA_TERM, "| |  | |              (_)               / /| ____|/ _ \\__ \\ \r\n");
    ACIA_WriteBuffer(ACIA_TERM, "| |__| | ___ _ __ _ __ _ _ __   __ _   / /_| |__ | | | | ) |\r\n");
    ACIA_WriteBuffer(ACIA_TERM, "|  __  |/ _ \\ '__| '__| | '_ \\ / _` | | '_ \\___ \\| | | |/ / \r\n");
    ACIA_WriteBuffer(ACIA_TERM, "| |  | |  __/ |  | |  | | | | | (_| | | (_) |__) | |_| / /_ \r\n");
    ACIA_WriteBuffer(ACIA_TERM, "|_|  |_|\\___|_|  |_|  |_|_| |_|\\__, |  \\___/____/ \\___/____|\r\n");
    ACIA_WriteBuffer(ACIA_TERM, "                                __/ |                       \r\n");
    ACIA_WriteBuffer(ACIA_TERM, "System Monitor v0.1            |___/    github.com/crmaykish\r\n");
    ACIA_WriteBuffer(ACIA_TERM, "############################################################\r\n");

    free_ram = _heapmaxavail();
    utoa(free_ram, buffer, 10);

    ACIA_WriteBuffer(ACIA_TERM, "Available RAM: ");
    ACIA_WriteBuffer(ACIA_TERM, buffer);
    ACIA_WriteBuffer(ACIA_TERM, " bytes");
    ACIA_NewLine(ACIA_TERM);

    poke(VIA1_DDRB, 0xFF);
    poke(VIA1_PORTB, 0x00);

    ACIA_WriteBuffer(ACIA_TERM, "Setting up SPI...");
    ACIA_NewLine(ACIA_TERM);

    // SPI Stuff
    SPI_Init();
    spi_data = SPI_ReadByte();
    poke(0xC401, 0b00001100);

    ACIA_WriteBuffer(ACIA_TERM, "Warmup Byte: ");
    PrintInt(spi_data);
    ACIA_NewLine(ACIA_TERM);

    spi_data = SPI_SendCommand(SPI_COMMAND_0);

    ACIA_WriteBuffer(ACIA_TERM, "CMD_0 Byte: ");
    PrintInt(spi_data);
    ACIA_NewLine(ACIA_TERM);

    spi_data = SPI_SendCommand(SPI_COMMAND_8);

    // ignore the 32-bit response
    SPI_ReadByte();
    SPI_ReadByte();
    SPI_ReadByte();
    SPI_ReadByte();

    ACIA_WriteBuffer(ACIA_TERM, "CMD_8 Byte: ");
    PrintInt(spi_data);
    ACIA_NewLine(ACIA_TERM);

    while (sd_init == 0)
    {
        spi_data = SPI_SendCommand(SPI_COMMAND_55);

        ACIA_WriteBuffer(ACIA_TERM, "CMD_55 Byte: ");
        PrintInt(spi_data);
        ACIA_NewLine(ACIA_TERM);

        spi_data = SPI_SendCommand(SPI_COMMAND_41);

        ACIA_WriteBuffer(ACIA_TERM, "CMD_41 Byte: ");
        PrintInt(spi_data);
        ACIA_NewLine(ACIA_TERM);

        if (spi_data == 0)
        {
            sd_init = 1;
        }
    }

    ACIA_WriteBuffer(ACIA_TERM, "SD card initialized.");
    ACIA_NewLine(ACIA_TERM);

    spi_data = SPI_SendCommand(SPI_COMMAND_17);

    if (spi_data == 0)
    {
        ACIA_WriteBuffer(ACIA_TERM, "Reading block 0 of SD card...");
        ACIA_NewLine(ACIA_TERM);
    }

    spi_data = 0xFF;

    while (spi_data != 0xFE)
    {
        spi_data = SPI_ReadByte();
    }

    spi_block = (byte *)malloc(512);
    memset(spi_block, 0, 512);

    ACIA_WriteBuffer(ACIA_TERM, "Start reading block data\r\n");

    for (spi_index = 0; spi_index < 512; spi_index++)
    {
        spi_block[spi_index] = SPI_ReadByte();
    }

    // disable SPI chip select
    poke(0xC401, 0b000000101);

    ACIA_WriteBuffer(ACIA_TERM, "First sector:\r\n");

    // memdump(spi_block, 32);

    ACIA_NewLine(ACIA_TERM);

    ACIA_WriteBuffer(ACIA_TERM, "Read partition table: \r\n");

    ACIA_WriteBuffer(ACIA_TERM, "first byte: ");
    PrintInt(spi_block[0x1BE]);

    ACIA_WriteBuffer(ACIA_TERM, ", start CHS: ");
    PrintInt(spi_block[0x1BE + 1]);
    PrintInt(spi_block[0x1BE + 2]);
    PrintInt(spi_block[0x1BE + 3]);

    ACIA_WriteBuffer(ACIA_TERM, ", partition type: ");
    PrintInt(spi_block[0x1BE + 4]);

    ACIA_WriteBuffer(ACIA_TERM, ", end CHS: ");
    PrintInt(spi_block[0x1BE + 5]);
    PrintInt(spi_block[0x1BE + 6]);
    PrintInt(spi_block[0x1BE + 7]);

    ACIA_WriteBuffer(ACIA_TERM, ", relative LBA addr: ");
    PrintInt(spi_block[0x1BE + 8]);
    PrintInt(spi_block[0x1BE + 9]);
    PrintInt(spi_block[0x1BE + 10]);
    PrintInt(spi_block[0x1BE + 11]);

    ACIA_WriteBuffer(ACIA_TERM, ", sectors long: ");
    PrintInt(spi_block[0x1BE + 12]);
    PrintInt(spi_block[0x1BE + 13]);
    PrintInt(spi_block[0x1BE + 14]);
    PrintInt(spi_block[0x1BE + 15]);

    ACIA_NewLine(ACIA_TERM);

    // TODO: reading additional blocks doesn't work correctly, response is C1/FF garbage, probably a clock issue again

    while (true)
    {
        ACIA_WriteBuffer(ACIA_TERM, "> ");
        ACIA_ReadLine(ACIA_TERM, buffer, 39, true);
        ACIA_NewLine(ACIA_TERM);

        ParseCommand(buffer);

        ACIA_NewLine(ACIA_TERM);

        memset(buffer, 0, 40);
    }

    return 0;
}
