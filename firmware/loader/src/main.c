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
            print("Commands: (h)elp, (l)oad, (r)un, (m)emdump");
            break;
        case 'l':
            print("Loading program from serial port...\r\n");
            bin_size = load_from_serial();

            print("\r\nLoading complete. Press 'r' to run.");
            break;
        case 'r':
            print("Running program from RAM...\r\n");
            run_loaded_program();
            break;
        case 'm':
            print("Dumping program RAM...\r\n");
            memdump(PROGRAM_RAM, bin_size);
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

    while (b != MAGIC_END_BYTE)
    {
        b = getc();
        // echo(b);
        POKE(PROGRAM_RAM + count, b);
        count++;
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
