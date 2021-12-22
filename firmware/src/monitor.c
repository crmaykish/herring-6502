#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <6502.h>
#include "herring.h"
#include "serial.h"

#define PROGRAM_VERSION "1.5.1"
#define RELEASE_DATE "2021-12-21"

#define PROMPT "><(((°>"

#define INPUT_BUFFER_SIZE 32
#define COMMAND_MAX_LENGTH 6
#define MEMDUMP_BYTES 128

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
uint8_t readline(char *buffer);

void memdump(uint16_t address, uint8_t bytes);

// Handler functions for each monitor command
void handler_help();
void handler_info();
void handler_peek();
void handler_poke();
void handler_dump();
void handler_run();
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
    {"run", "", "", "Jump to program RAM (0x1000)", handler_run},
    {"zero", "", "", "Clear program memory (0x1000-0x7FFF)", handler_zero},
    {"load", "addr", "", "Load a program over serial", handler_load},
    {"clear", "", "", "Clear the screen", handler_clear}};

static const uint8_t COMMAND_COUNT = sizeof(commands) / sizeof(command_t);

int main()
{
    uint8_t i;
    char buffer[INPUT_BUFFER_SIZE];
    char *command;
    bool command_handled;

    serial_init();
    header();

    while (true)
    {
        command_handled = false;

        // Present the command prompt and wait for input
        prompt();
        readline(buffer);
        print_line(NULL);

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

        print_line(NULL);
    }

    return 0;
}

void header()
{
    term_clear();

    term_set_color(TERM_FG_BRIGHT_BLUE);
    puts("..·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·..\r\n");

    term_set_color(TERM_FG_BRIGHT_GREEN);
    puts("Herring Monitor " PROGRAM_VERSION " | Colin Maykish | Built: " RELEASE_DATE "\r\n");

    term_set_color(TERM_FG_BRIGHT_RED);
    puts("github.com/crmaykish/herring-6502\r\n");

    term_set_color(TERM_FG_BRIGHT_BLUE);
    puts("..·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·..\r\n\r\n");

    term_set_color(TERM_RESET);
}

void prompt()
{
    term_set_color(TERM_FG_CYAN);
    puts(PROMPT);
    term_set_color(TERM_RESET);
    puts(" ");
}

void free_ram()
{
    size_t free_ram = _heapmemavail();

    print_dec(free_ram);
    puts(" bytes free");
}

void handler_help()
{
    uint8_t i = 0;

    print_line("Available commands:");

    for (i = 0; i < COMMAND_COUNT; i++)
    {
        term_set_color(TERM_FG_BRIGHT_GREEN);
        puts(commands[i].name);
        term_set_color(TERM_RESET);

        if (commands[i].param1[0] != '\0')
        {
            puts(" <");
            puts(commands[i].param1);
            puts(">");
        }
        if (commands[i].param2[0] != '\0')
        {
            puts(" <");
            puts(commands[i].param2);
            puts(">");
        }

        puts(" : ");

        term_set_color(TERM_FG_CYAN);
        puts(commands[i].desc);
        term_set_color(TERM_RESET);

        if (i != COMMAND_COUNT - 1)
        {
            print_line(NULL);
        }
    }
}

void handler_info()
{
    puts("CPU: ");
    puts(cpu_names[getcpu()]);
    puts("\r\nRAM: ");
    free_ram();
}

void handler_peek()
{
    char *param1 = strtok(NULL, " ");
    uint16_t addr = strtol(param1, 0, 16);

    print_hex(PEEK(addr));
}

void handler_poke()
{
    char *param1 = strtok(NULL, " ");
    char *param2 = strtok(NULL, " ");

    uint16_t addr = strtol(param1, 0, 16);
    uint8_t val = strtol(param2, 0, 16);

    POKE(addr, val);
    puts("OK");
}

void handler_dump()
{
    char *param1 = strtok(NULL, " ");
    uint16_t addr = strtol(param1, 0, 16);

    memdump(addr, MEMDUMP_BYTES);
}

void handler_run()
{
    __asm__("jsr %w", PROGRAM_RAM);
}

void handler_zero()
{
    memset((uint16_t *)0x1000, 0, 0x7000);
    puts("OK");
}

void handler_load()
{
    uint16_t in_count = 0;
    uint8_t magic_count = 0;
    uint8_t in = 0;
    char *param1 = strtok(NULL, " ");
    uint16_t addr = strtol(param1, 0, 16);

    puts("Loading into: 0x");
    print_hex(addr);
    print_line("...");

    while (magic_count != 3)
    {
        in = getc();

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

    // Remove the magic bytes from the end of the firmware in RAM
    memset((uint16_t *)(addr + in_count - 3), 0, 3);

    print_dec(in_count - 3);
    print_line(" bytes read");
    puts("Done!");
}

void handler_clear()
{
    term_clear();
}

void command_not_found(char *command_name)
{
    term_set_color(TERM_FG_RED);
    puts("Command not found: ");
    puts(command_name);
    term_set_color(TERM_RESET);
}

uint8_t readline(char *buffer)
{
    uint8_t count = 0;
    uint8_t in = getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            putc(in);

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

                term_cursor_move(TERM_CURSOR_LEFT, 1);
                putc(' ');
                term_cursor_move(TERM_CURSOR_LEFT, 1);
            }
        }

        in = getc();
    }

    buffer[count] = 0;

    return count;
}

void memdump(uint16_t address, uint8_t bytes)
{
    uint16_t i = 0;
    uint8_t b = 0;

    print_hex(address);
    puts("  ");

    while (i < bytes)
    {
        b = PEEK(address + i);
        print_hex(b);
        putc(' ');

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            puts(" |");
            print_string_bin((char *)(address + i - 16), 16);

            puts("|\r\n");
            print_hex(address + i);
            puts("  ");
        }
        else if (i % 8 == 0)
        {
            putc(' ');
        }
    }

    putc('|');
    print_string_bin((char *)(address + i - 16), 16);
    putc('|');
}