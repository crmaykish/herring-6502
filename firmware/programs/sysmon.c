#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "via.h"
#include "spi.h"
#include "utils.h"

void memdump(word addr, byte rows)
{
    byte data;
    word index = 0;
    byte i = 0;
    byte j = 0;

    if (rows == 0)
    {
        rows = 8;
    }

    for (j; j < rows; j++)
    {
        print_hex(addr + index);
        print("  ");

        for (i = 0; i < 16; i++)
        {
            data = peek(addr + index);
            print_hex(data);

            if (i == 7)
            {
                put(' ');
            }

            put(' ');

            index++;
        }

        index -= 16;

        // Write the ASCII characters
        print("  |");
        for (i = 0; i < 16; i++)
        {
            data = peek(addr + index);

            if (data >= 32 && data < 127)
            {
                put(data);
            }
            else
            {
                put('.');
            }

            index++;
        }

        print("|\r\n");
    }
}

void ParseCommand(char *buffer)
{
    word addr;
    byte data;

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
        print_hex(peek(addr));
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
}

int main()
{
    char buffer[40];
    byte spi_data = 0;
    byte i = 0;
    byte sd_init = 0;
    word spi_index = 0;
    byte *spi_block;

    ACIA_Init(&SerialConsole);

    // print("############################################################\r\n");
    // print(" _    _                _                 __ _____  ___ ___  \r\n");
    // print("| |  | |              (_)               / /| ____|/ _ \\__ \\ \r\n");
    // print("| |__| | ___ _ __ _ __ _ _ __   __ _   / /_| |__ | | | | ) |\r\n");
    // print("|  __  |/ _ \\ '__| '__| | '_ \\ / _` | | '_ \\___ \\| | | |/ / \r\n");
    // print("| |  | |  __/ |  | |  | | | | | (_| | | (_) |__) | |_| / /_ \r\n");
    // print("|_|  |_|\\___|_|  |_|  |_|_| |_|\\__, |  \\___/____/ \\___/____|\r\n");
    // print("                                __/ |                       \r\n");
    // print("System Monitor v0.1            |___/    github.com/crmaykish\r\n");
    // print("############################################################\r\n");

    print("Herring 6502 \r\n");

    poke(VIA1_DDRB, 0xFF);
    poke(VIA1_PORTB, 0x00);

    print("Setting up SPI...\r\n");

    // SPI Stuff
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

    spi_data = SPI_SendCommand(SPI_COMMAND_17);

    if (spi_data == 0)
    {
        print("Reading block 0 of SD card...\r\n");
    }

    spi_data = 0xFF;

    while (spi_data != 0xFE)
    {
        spi_data = SPI_ReadByte();
    }

    spi_block = (byte *)malloc(512);

    print("Start reading block data\r\n");

    for (spi_index = 0; spi_index < 512; spi_index++)
    {
        spi_block[spi_index] = SPI_ReadByte();
    }

    // disable SPI chip select
    poke(0xC401, 0b000000101);

    print("First sector:\r\n");

    // memdump(spi_block, 32);

    print("Read partition table: \r\n");

    print("first byte: ");
    print_hex(spi_block[0x1BE]);

    print(", start CHS: ");
    print_hex(spi_block[0x1BE + 1]);
    print_hex(spi_block[0x1BE + 2]);
    print_hex(spi_block[0x1BE + 3]);

    print(", partition type: ");
    print_hex(spi_block[0x1BE + 4]);

    print(", end CHS: ");
    print_hex(spi_block[0x1BE + 5]);
    print_hex(spi_block[0x1BE + 6]);
    print_hex(spi_block[0x1BE + 7]);

    print(", relative LBA addr: ");
    print_hex(spi_block[0x1BE + 8]);
    print_hex(spi_block[0x1BE + 9]);
    print_hex(spi_block[0x1BE + 10]);
    print_hex(spi_block[0x1BE + 11]);

    print(", sectors long: ");
    print_hex(spi_block[0x1BE + 12]);
    print_hex(spi_block[0x1BE + 13]);
    print_hex(spi_block[0x1BE + 14]);
    print_hex(spi_block[0x1BE + 15]);

    print("\r\n");

    // TODO: reading additional blocks doesn't work correctly, response is C1/FF garbage, probably a clock issue again

    while (true)
    {
        print("> ");
        ACIA_ReadLine(&SerialConsole, buffer, 39, true);
        print("\r\n");
        ParseCommand(buffer);
        print("\r\n");
    }

    return 0;
}
