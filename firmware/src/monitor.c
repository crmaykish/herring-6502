#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <peekpoke.h>
#include <6502.h>
#include "herring.h"
#include "serial.h"

#define RELEASE_DATE "2024-06-29"

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
        printf("\r\n");

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

        printf("\r\n");
    }

    return 0;
}

void header()
{
    term_clear();

    term_set_color(TERM_FG_BRIGHT_BLUE);
    printf("..·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·..\r\n");

    term_set_color(TERM_FG_BRIGHT_GREEN);
    printf("  _  _             _              __ ___  __ ___\r\n");
    printf(" | || |___ _ _ _ _(_)_ _  __ _   / /| __|/  \\_  )\r\n");
    printf(" | __ / -_) '_| '_| | ' \\/ _` | / _ \\__ \\ () / /\r\n");
    printf(" |_||_\\___|_| |_| |_|_||_\\__, | \\___/___/\\__/___|\r\n");
    printf("  Colin Maykish          |___/   " RELEASE_DATE "\r\n");

    term_set_color(TERM_FG_BRIGHT_RED);
    printf("github.com/crmaykish/herring-6502\r\n");

    term_set_color(TERM_FG_BRIGHT_BLUE);
    printf("..·´¯`·..·´¯`·..·´¯`·..·´¯`·..·´¯`·..´¯`·..·´¯`·..·´¯`·..\r\n\r\n");

    term_set_color(TERM_RESET);
}

void prompt()
{
    term_set_color(TERM_FG_CYAN);
    printf("%s ", PROMPT);
    term_set_color(TERM_RESET);
}

void free_ram()
{
    printf("%d bytes free", _heapmemavail());
}

void handler_help()
{
    uint8_t i = 0;

    printf("Available commands:\r\n");

    for (i = 0; i < COMMAND_COUNT; i++)
    {
        term_set_color(TERM_FG_BRIGHT_GREEN);
        printf(commands[i].name);
        term_set_color(TERM_RESET);

        if (commands[i].param1[0] != '\0')
        {
            printf(" <");
            printf(commands[i].param1);
            printf(">");
        }
        if (commands[i].param2[0] != '\0')
        {
            printf(" <");
            printf(commands[i].param2);
            printf(">");
        }

        printf(" : ");

        term_set_color(TERM_FG_CYAN);
        printf(commands[i].desc);
        term_set_color(TERM_RESET);

        if (i != COMMAND_COUNT - 1)
        {
            printf("\r\n");
        }
    }
}

void handler_info()
{
    printf("CPU: ");
    printf(cpu_names[getcpu()]);
    printf("\r\nRAM: ");
    free_ram();
}

void handler_peek()
{
    char *param1 = strtok(NULL, " ");
    uint16_t addr = strtol(param1, 0, 16);

    printf("%02X", PEEK(addr));
}

void handler_poke()
{
    char *param1 = strtok(NULL, " ");
    char *param2 = strtok(NULL, " ");

    uint16_t addr = strtol(param1, 0, 16);
    uint8_t val = strtol(param2, 0, 16);

    POKE(addr, val);
    printf("OK");
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
    printf("OK");
}

void handler_load()
{
    uint16_t in_count = 0;
    uint8_t magic_count = 0;
    uint8_t in = 0;
    char *param1 = strtok(NULL, " ");
    uint16_t addr = strtol(param1, 0, 16);

    printf("Loading into: 0x%04X...", addr);

    while (magic_count != 3)
    {
        in = serial_getc();

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

    printf("%d bytes read\r\nDone!", in_count - 3);
}

void handler_clear()
{
    term_clear();
}

void command_not_found(char *command_name)
{
    term_set_color(TERM_FG_RED);
    printf("Command not found: ");
    printf(command_name);
    term_set_color(TERM_RESET);
}

uint8_t readline(char *buffer)
{
    uint8_t count = 0;
    uint8_t in = serial_getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            serial_putc(in);

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
                serial_putc(' ');
                term_cursor_move(TERM_CURSOR_LEFT, 1);
            }
        }

        in = serial_getc();
    }

    buffer[count] = 0;

    return count;
}

void memdump(uint16_t address, uint8_t bytes)
{
    uint16_t i = 0;
    uint8_t b = 0;

    printf("%04X  ", address);

    while (i < bytes)
    {
        b = PEEK(address + i);
        printf("%02X ", b);

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            printf(" |");
            print_string_bin((char *)(address + i - 16), 16);

            printf("|\r\n%04X  ", address + i);
        }
        else if (i % 8 == 0)
        {
            serial_putc(' ');
        }
    }

    serial_putc('|');
    print_string_bin((char *)(address + i - 16), 16);
    serial_putc('|');
}