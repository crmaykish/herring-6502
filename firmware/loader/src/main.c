#include "via.h"
#include "acia.h"

// TODO: Add the status LED stuff back to the crt0 files, try to figure out what is going on here

// TODO: Update the config to use only the lower RAM since high RAM is now empty

void memdump(word addr, word count);

int main()
{
    // Serial input byte
    byte in = 0;
    // Number of bytes read
    word in_count = 0;

    word i = 0;

    bool loading = true;

    via_init();
    acia_init();

    print("\r\nReady\r\n");

    while (true)
    {
        // Wait for a byte from the serial port
        in = getc();

        if (in == STOP_BYTE)
        {
            loading = false;
        }

        if (loading)
        {
            POKE(PROGRAM_ADDR + in_count, in);
            in_count++;
            via_set_word(in_count);
        }
        else
        {
            print_int(in_count);
            print(" BYTES\r\n");

            memdump(PROGRAM_ADDR, in_count);

            print("\r\nRUN\r\n");
            jump_to_ram();
        }
    }

    return 0;
}

void memdump(word addr, word count)
{
    word i = 0;

    print_hex(addr);
    putc(' ');

    for (i; i < count; ++i)
    {
        // Print hex value
        print_hex(PEEK(addr + i));
        putc(' ');

        if (i > 0)
        {
            if (i % 8 == 0)
            {
                putc(' ');
            }
            if (i % 16 == 0)
            {
                // Print newline and address
                print("\r\n");
                print_hex(addr + i);
                putc(' ');
                putc(' ');
            }
        }
    }
}
