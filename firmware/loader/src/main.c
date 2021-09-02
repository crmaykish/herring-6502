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
#include "jump.h"

#define INPUT_BUFFER_SIZE 80

void header();
void free_ram();
void load_code(word addr);

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

        if (strncmp(buffer, "help", 4) == 0)
        {
            print_line("Commands:");
            print_line("help");
            print_line("clear");
            print_line("free");
            print_line("peek <addr>");
            print_line("poke <addr> <val>");
            print_line("dump <addr>");
            print_line("jump <addr>");
            print_line("load <addr>");
            print("zero <addr>");
        }
        else if (strncmp(buffer, "peek", 4) == 0)
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
        else if (strncmp(buffer, "jump", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            jump_to(addr);
        }
        else if (strncmp(buffer, "zero", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            memset((word *)addr, 0, 128);

            print("OK");
        }
        else if (strncmp(buffer, "free", 4) == 0)
        {
            free_ram();
        }
        else if (strncmp(buffer, "clear", 5) == 0)
        {
            screen_clear();
            continue;
        }
        else if (strncmp(buffer, "load", 4) == 0)
        {
            addr = strtol(&buffer[4], 0, 16);
            print("Loading into: 0x");
            print_hex(addr);
            print_line("...");

            load_code(addr);
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
    free_ram();
    print_newline();

    font_reset();

    print("-----\r\n");
}

void free_ram()
{
    size_t free_ram = _heapmemavail();

    print_dec(free_ram);
    print(" bytes free");
}

void load_code(word addr)
{
    word in_count = 0;
    byte magic_count = 0;
    byte in = 0;

    while (magic_count != 3)
    {
        in = acia_getc();

        // Don't store the first DE magic byte that comes in
        if (in == 0xDE && in_count == 0)
        {
            continue;
        }

        POKE(addr + in_count, in);

        if (in == 0xDE)
        {
            magic_count++;
        }
        else
        {
            magic_count = 0;
        }

        in_count++;
    }

    print_line("Done loading!");
}