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

#define INPUT_BUFFER_SIZE 32
#define COMMAND_MAX_LENGTH 6
#define MEMDUMP_BYTES 128

#define PROMPT "><(((°>"

typedef struct
{
    char name[6];
    char param1[6];
    char param2[6];
    char desc[48];
    void (*handler)();
} command_t;

void header();
void prompt();
void free_ram();
byte readline(char *buffer);

// Handler functions for each monitor command
void handler_help();
void handler_info();
void handler_peek();
void handler_poke();
void handler_dump();
void handler_jump();
void handler_zero();
void handler_load();
void handler_clear();
void command_not_found(char *command_name);

// CPU type names
static const char *cpu_names[] = {"6502", "65C02", "65816", "4510", "65SC02", "65CE02"};

// Input command definitions
static const command_t commands[] = {
    {"help", "", "", "Show this message", handler_help},
    {"info", "", "", "Get system info", handler_info},
    {"peek", "addr", "", "Peek at a memory address", handler_peek},
    {"poke", "addr", "val", "Poke a value into memory", handler_poke},
    {"dump", "addr", "", "Dump memory in hex and ASCII", handler_dump},
    {"jump", "addr", "", "Jump to address", handler_jump},
    {"zero", "", "", "Clear program memory (0x1000-0x7FFF)", handler_zero},
    {"load", "addr", "", "Load a program over serial", handler_load},
    {"clear", "", "", "Clear the screen", handler_clear}};

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
        readline(buffer);
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
        font_green();
        print(commands[i].name);
        font_reset();

        if (commands[i].param1[0] != '\0')
        {
            print(" <");
            print(commands[i].param1);
            print(">");
        }
        if (commands[i].param2[0] != '\0')
        {
            print(" <");
            print(commands[i].param2);
            print(">");
        }

        print(" : ");

        font_cyan();
        print(commands[i].desc);
        font_reset();

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
    memset((word *)0x1000, 0, 0x7000);
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

void handler_clear()
{
    screen_clear();
}

void command_not_found(char *command_name)
{
    font_red();
    print("Command not found: ");
    print(command_name);
    font_reset();
}

byte readline(char *buffer)
{
    byte count = 0;
    byte in = acia_getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            acia_putc(in);

            buffer[count] = in;
            count++;
        }

        // Character is backspace
        else if (in == 0x7F)
        {
            if (count != 0)
            {
                // Move cursor back one character and clear the previous character
                count--;
                buffer[count] = '\0';

                cursor_move(CURSOR_LEFT, 1);
                acia_putc(' ');
                cursor_move(CURSOR_LEFT, 1);
            }
        }

        in = acia_getc();
    }

    buffer[count] = 0;

    return count;
}
