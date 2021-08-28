#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <peekpoke.h>
#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"
#include "memdump.h"

#define INPUT_BUFFER_SIZE 80

void header();

int main()
{
    char buffer[INPUT_BUFFER_SIZE];
    word addr = 0;
    byte val = 0;

    acia_init();
    header();

    while (true)
    {
        print("> ");
        readline(buffer, true);
        print_newline();

        if (strncmp(buffer, "peek", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            print_hex(PEEK(addr));
        }
        else if (strncmp(buffer, "poke", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            val = strtol(&buffer[9], 0, 16);
            POKE(addr, val);
            print("OK");
        }
        else if (strncmp(buffer, "dump", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            memdump(addr, 128);
        }
        else
        {
            font_red();
            print("Command not found: ");
            print(buffer);
            font_reset();
        }

        print_newline();
    }

    return 0;
}

void header()
{
    size_t free_ram = _heapmemavail();

    screen_clear();

    font_green();
    print("Herring 6502 Monitor v1.2\r\n");

    font_red();
    print("Colin Maykish - 2021\r\n");

    font_cyan();
    print("github.com/crmaykish/herring-6502\r\n");

    font_reset();

    print("-----\r\n");

    font_blue();
    print("Available RAM: ");
    print_dec(free_ram);
    print(" bytes\r\n");

    font_reset();

    print("-----\r\n");
}