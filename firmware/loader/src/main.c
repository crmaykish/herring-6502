#include <string.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "loader.h"

#define PROGRAM_RAM 0x1000
#define VIA_PORT 0xC000
#define MAGIC_END_BYTE 0xDE

void memdump(unsigned int address, word count);
word load_from_serial();
void memtest(word start, word size);
void echo(byte c);

int main()
{
    byte in = 0;
    word bin_size = 0;

    acia_init();

    print("\r\n\r\n==========================\r\n");
    print("\r\nHerring 6502\r\n");
    print("Serial Bootloader v2.0\r\n");

    print("Zeroing out program RAM... ");
    memset((void *)PROGRAM_RAM, 0, 0xC000 - PROGRAM_RAM);
    print("DONE\r\n\r\n");

    while (true)
    {
        print("> ");
        in = getc();

        putc(in);

        print("\r\n");

        switch (in)
        {
        case 'h':
            // Print the help message
            print("Commands: (h)elp, (l)oad, (r)un, (m)emdump, (e)rase");
            break;
        case 'l':
            print("Loading program from serial port...\r\n");
            bin_size = load_from_serial();

            print("\r\nLoading complete. Press 'r' to run.");
            break;
        case 'r':
            if (bin_size > 0)
            {
                print("Running program from RAM...\r\n");
                run_loaded_program();
            }
            else
            {
                print("No program has been loaded.\r\n");
            }
            break;
        case 'e':
            print("Erasing program...");
            memset((void *)PROGRAM_RAM, 0, bin_size);
            bin_size = 0;
            print("DONE");
            break;
        case 'm':
            if (bin_size > 0)
            {
                print("Dumping program RAM...\r\n");
                memdump(PROGRAM_RAM, bin_size);
            }
            else
            {
                print("No program has been loaded.\r\n");
            }
            break;
        case 't':
            print("Running memtest...\r\n");
            memtest(PROGRAM_RAM, 0xC000 - PROGRAM_RAM);
            print("DONE");
            break;
        default:
            print("Command not found.");
            break;
        }

        print("\r\n");
    }

    return 0;
}

void memdump(unsigned int address, word count)
{
    word i = 0;
    byte b = 0;

    while (i < count)
    {
        b = PEEK(address + i);
        echo(b);

        i++;
    }
}

word load_from_serial()
{
    byte b = 0;
    word count = 0;
    byte magic_count = 0;

    while (magic_count != 3)
    {
        b = getc();
        POKE(PROGRAM_RAM + count, b);
        count++;

        if (b == MAGIC_END_BYTE)
        {
            magic_count++;
        }
        else
        {
            magic_count = 0;
        }
    }

    return count;
}

void echo(byte c)
{
    if (c >= 32 && c < 127)
    {
        putc(c);
    }
    else
    {
        putc('.');
    }
}

void memtest(word start, word size)
{
    byte b = 0;
    word i = 0;

    while (i < size)
    {
        POKE(start + i, 0x00);

        b = PEEK(start + i);

        if (b != 0x00)
        {
            print("Mem failure\r\n");
        }

        i++;
    }
}
