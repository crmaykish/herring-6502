#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

void vga_clear_screen();

void vga_print(char *s);

void vga_putc(char b);

int main()
{
    char in;

    print_line("\r\n| === VGA Text Mode Demo === |\r\n");

    vga_print("herring computer system\r\n");
    vga_print("colin maykish\r\n");

    while (true)
    {
        in = acia_getc();

        if (in == 27)
        {
            vga_clear_screen();
        }
        if (in == 0x0D)
        {
            vga_putc(0x0D);
            vga_putc(0x0A);
        }
        else
        {
            vga_putc(in);
        }

        print_dec(in);
        print_newline();
    }

    return 0;
}

void vga_clear_screen()
{
    POKE(0x9000, 0x7F); // clear screen
}

void vga_putc(char b)
{
    if (b == 0x0D || b == 0x0A)
    {
        POKE(0x9000, b);
    }
    else if (b == 32)
    {
        // space
        POKE(0x9000, 0);
    }
    else if (b >= 65 && b < 128)
    {
        // visible ASCII
        POKE(0x9000, b - 64);
    }
}

void vga_print(char *s)
{
    byte i = 0;
    byte b = s[0];

    while (b != 0)
    {
        vga_putc(b);

        i++;
        b = s[i];
    }
}