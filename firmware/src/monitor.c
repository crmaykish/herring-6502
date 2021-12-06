#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <6502.h>
#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"
#include "memdump.h"
#include "jump.h"

#define INPUT_BUFFER_SIZE 80

static const char *cpu_names[] = {"6502", "65C02", "65816", "4510", "65SC02", "65CE02"};

static const char *delims = " ";

void header();
void free_ram();
void load_code(word addr);

int main()
{
    char buffer[INPUT_BUFFER_SIZE];
    char *command;
    char *param1;
    char *param2;
    word addr = 0;
    byte val = 0;

    acia_init();
    header();

    while (true)
    {
        print("> ");
        readline(buffer, true);
        print_newline();

        command = strtok(buffer, delims);

        if (command != NULL)
        {
            param1 = strtok(NULL, delims);

            if (param1 != NULL)
            {
                param2 = strtok(NULL, delims);
            }
        }

        if (strcmp(command, "help") == 0)
        {
            print_line("Commands:");
            print_line("help");
            print_line("clear");
            print_line("info");
            print_line("peek <addr>");
            print_line("poke <addr> <val>");
            print_line("dump <addr>");
            print_line("jump <addr>");
            print_line("load <addr>");
            print("zero <addr>");
        }
        else if (strcmp(command, "peek") == 0)
        {
            addr = strtol(param1, 0, 16);
            print_hex(PEEK(addr));
        }
        else if (strcmp(command, "poke") == 0)
        {
            addr = strtol(param1, 0, 16);
            val = strtol(param2, 0, 16);
            POKE(addr, val);
            print("OK");
        }
        else if (strcmp(command, "dump") == 0)
        {
            addr = strtol(param1, 0, 16);
            memdump(addr, 128);
        }
        else if (strcmp(command, "jump") == 0)
        {
            addr = strtol(param1, 0, 16);
            jump_to(addr);
        }
        else if (strcmp(command, "zero") == 0)
        {
            addr = strtol(param1, 0, 16);
            memset((word *)addr, 0, 128);

            print("OK");
        }
        else if (strcmp(command, "info") == 0)
        {
            print("CPU: ");
            print(cpu_names[getcpu()]);
            print("\r\nRAM: ");
            free_ram();
        }
        else if (strcmp(command, "clear") == 0)
        {
            screen_clear();
            continue;
        }
        else if (strcmp(command, "load") == 0)
        {
            addr = strtol(param1, 0, 16);
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

    print("=======================================================\r\n");

    font_green();
    print("Herring Monitor v1.3 | Colin Maykish | Built 2021-12-01\r\n");

    font_cyan();
    print("github.com/crmaykish/herring-6502\r\n");

    font_reset();

    print("=======================================================\r\n\r\n");
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