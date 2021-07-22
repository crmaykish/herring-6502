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

void ReadPartitionTable(byte *block)
{
    print("first byte: ");
    print_hex(block[0x1BE]);

    print(", start CHS: ");
    print_hex(block[0x1BE + 1]);
    print_hex(block[0x1BE + 2]);
    print_hex(block[0x1BE + 3]);

    print(", partition type: ");
    print_hex(block[0x1BE + 4]);

    print(", end CHS: ");
    print_hex(block[0x1BE + 5]);
    print_hex(block[0x1BE + 6]);
    print_hex(block[0x1BE + 7]);

    print(", relative LBA addr: ");
    print_hex(block[0x1BE + 8]);
    print_hex(block[0x1BE + 9]);
    print_hex(block[0x1BE + 10]);
    print_hex(block[0x1BE + 11]);

    print(", sectors long: ");
    print_hex(block[0x1BE + 12]);
    print_hex(block[0x1BE + 13]);
    print_hex(block[0x1BE + 14]);
    print_hex(block[0x1BE + 15]);

    print("\r\n");
}

int main()
{
    char buffer[40];
    byte i = 0;

    byte *block = (byte *)malloc(SD_BLOCK_SIZE);

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

    print("Setting up VIAs...\r\n");
    VIA_InitAll();

    print("Setting up SD card...\r\n");
    SD_Init();

    SD_ReadBlock(block, 0, 0, 0, 0);

    print("Read partition table: \r\n");
    ReadPartitionTable(block);

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
