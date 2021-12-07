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
#define COMMAND_MAX_LENGTH 6
#define MEMDUMP_BYTES 128

#define PROMPT "><(((°>"

typedef struct
{
    char name[6];
    char param1[6];
    char param2[6];
    void (*handler)();
} command_t;

void header();
void prompt();
void free_ram();

// Handler functions for each monitor command
void handler_help();
void handler_info();
void handler_peek();
void handler_poke();
void handler_dump();
void handler_jump();
void handler_zero();
void handler_load();
void command_not_found(char *command_name);

// CPU type names
static const char *cpu_names[] = {"6502", "65C02", "65816", "4510", "65SC02", "65CE02"};

// Input command definitions
static const command_t commands[] = {
    {"help", "", "", handler_help},
    {"info", "", "", handler_info},
    {"peek", "addr", "", handler_peek},
    {"poke", "addr", "val", handler_poke},
    {"dump", "addr", "", handler_dump},
    {"jump", "addr", "", handler_jump},
    {"zero", "addr", "", handler_zero},
    {"load", "addr", "", handler_load}};

static const byte COMMAND_COUNT = sizeof(commands) / sizeof(command_t);

int main()
{
    byte i;
    char buffer[INPUT_BUFFER_SIZE];
    char *command;
    bool command_handled;

    acia_init();
    header();

    while (true)
    {
        command_handled = false;

        // Present the command prompt and wait for input
        prompt();
        readline(buffer, true);
        print_newline();

        command = strtok(buffer, " ");

        // Look for the command name in the command list
        for (i = 0; i < COMMAND_COUNT; i++)
        {
            if (strncmp(command, commands[i].name, COMMAND_MAX_LENGTH) == 0)
            {
                // Found the command, handle it
                commands[i].handler();
                command_handled = true;
                break;
            }
        }

        if (!command_handled)
        {
            command_not_found(command);
        }

        print_newline();
    }

    return 0;
}

void header()
{
    screen_clear();

    font_blue();
    print(".·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·.\r\n");

    font_green();
    print("Herring Monitor v1.4 | Colin Maykish | Built 2021-12-06\r\n");

    font_red();
    print("github.com/crmaykish/herring-6502\r\n");

    font_blue();
    print(".·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·.\r\n\r\n");

    font_reset();
}

void prompt()
{
    font_cyan();
    print(PROMPT);
    font_reset();
    print(" ");
}

void free_ram()
{
    size_t free_ram = _heapmemavail();

    print_dec(free_ram);
    print(" bytes free");
}

void handler_help()
{
    byte i = 0;

    print_line("Available commands:");

    for (i = 0; i < COMMAND_COUNT; i++)
    {
        print(commands[i].name);
        if (commands[i].param1[0] != '\0')
        {
            print(" <");
            print(commands[i].param1);
            print(">");
        }
        if (commands[i].param2[0] != '\0')
        {
            print(" <");
            print(commands[i].param1);
            print(">");
        }

        if (i != COMMAND_COUNT - 1)
        {
            print_newline();
        }
    }
}

void handler_info()
{
    print("CPU: ");
    print(cpu_names[getcpu()]);
    print("\r\nRAM: ");
    free_ram();
}

void handler_peek()
{
    char *param1 = strtok(NULL, " ");
    word addr = strtol(param1, 0, 16);

    print_hex(PEEK(addr));
}

void handler_poke()
{
    char *param1 = strtok(NULL, " ");
    char *param2 = strtok(NULL, " ");

    word addr = strtol(param1, 0, 16);
    byte val = strtol(param2, 0, 16);

    POKE(addr, val);
    print("OK");
}

void handler_dump()
{
    char *param1 = strtok(NULL, " ");
    word addr = strtol(param1, 0, 16);

    memdump(addr, MEMDUMP_BYTES);
}

void handler_jump()
{
    char *param1 = strtok(NULL, " ");
    word addr = strtol(param1, 0, 16);
    jump_to(addr);
}

void handler_zero()
{
    char *param1 = strtok(NULL, " ");
    word addr = strtol(param1, 0, 16);
    memset((word *)addr, 0, MEMDUMP_BYTES);

    print("OK");
}

void handler_load()
{
    word in_count = 0;
    byte magic_count = 0;
    byte in = 0;
    char *param1 = strtok(NULL, " ");
    word addr = strtol(param1, 0, 16);

    print("Loading into: 0x");
    print_hex(addr);
    print_line("...");

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

    print("Done!");
}

void command_not_found(char *command_name)
{
    font_red();
    print("Command not found: ");
    print(command_name);
    font_reset();
}