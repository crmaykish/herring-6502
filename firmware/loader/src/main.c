#include <stdlib.h>
#include <string.h>
#include <peekpoke.h>
#include <stdbool.h>
#include "herring.h"
#include "loader.h"
#include "acia.h"
#include "print.h"
#include "assembler.h"

#define PROGRAM_RAM 0x1000
#define FILE_RAM 0x8000
#define MAGIC_END_BYTE 0xDE
#define LOADER_BUFFER_LEN 40

typedef struct
{
    char buffer[LOADER_BUFFER_LEN];
    word binary_size;
} loader_t;

typedef struct
{
    char name[6];
    byte len;
    void (*handler)(loader_t *);
} command_t;

void interpret(loader_t *loader);

// Command handlers
void help(loader_t *loader);
void load(loader_t *loader);
void run(loader_t *loader);
void dump(loader_t *loader);
void erase(loader_t *loader);
void dis(loader_t *loader);
void edit(loader_t *loader);
void cat(loader_t *loader);
void as(loader_t *loader);

static command_t commands[] = {
    {"help", 4, help},
    {"load", 4, load},
    {"run", 3, run},
    {"dump", 4, dump},
    {"erase", 5, erase},
    {"da", 3, dis},
    {"edit", 4, edit},
    {"cat", 3, cat},
    {"as", 2, as}};

int main()
{
    loader_t loader;
    loader.binary_size = 0;
    memset(loader.buffer, 0, LOADER_BUFFER_LEN);

    acia_init();

    print("\r\nHerring 6502\r\n");

    while (true)
    {
        print("> ");
        readline(loader.buffer, true);
        print("\r\n");

        interpret(&loader);

        print("\r\n");
    }

    return 0;
}

void interpret(loader_t *loader)
{
    byte i;

    for (i = 0; i < sizeof(commands) / sizeof(command_t); i++)
    {
        if (strncmp(loader->buffer, commands[i].name, commands[i].len) == 0)
        {
            // Found the command, run the handler
            commands[i].handler(loader);
            return;
        }
    }

    print("Command not found");
}

void help(loader_t *loader)
{
    byte i;

    for (i = 0; i < sizeof(commands) / sizeof(command_t); i++)
    {
        print(commands[i].name);
        // print(": ");
        // print(commands[i].desc);
        print("\r\n");
    }
}

void load(loader_t *loader)
{
    byte b = 0;
    byte magic_count = 0;
    loader->binary_size = 0;

    while (magic_count != 3)
    {
        b = getc();
        POKE(PROGRAM_RAM + loader->binary_size, b);
        loader->binary_size++;

        if (b == MAGIC_END_BYTE)
        {
            magic_count++;
        }
        else
        {
            magic_count = 0;
        }
    }
}

void run(loader_t *loader)
{
    run_loaded_program();
}

void dump(loader_t *loader)
{
    word i = 0;
    byte b = 0;

    while (i < 240)
    {
        b = PEEK(PROGRAM_RAM + i);
        if (b >= 32 && b < 127)
        {
            putc(b);
        }
        else
        {
            putc('.');
        }

        i++;
    }
}

void erase(loader_t *loader)
{
    memset((void *)PROGRAM_RAM, 0, 0xC000 - PROGRAM_RAM);
    loader->binary_size = 0;
}

void dis(loader_t *loader)
{
    word i = 0;
    op_code_t *curr = NULL;
    byte *program = (byte *)PROGRAM_RAM;

    while (i < loader->binary_size)
    {
        print_hex((word)program + i);
        print(": ");

        curr = opcode_lookup(program[i]);

        if (curr != NULL)
        {
            // Print the mnemonic
            print(curr->mnemonic);
            putc(' ');

            // Print any needed prefixes of the operand
            switch (curr->mode)
            {
            case IMM:
                print("#$");
                break;
            case ABS:
                putc('$');
                break;
            case REL:
                putc('$');
                break;
            default:
                break;
            }

            // Print the operand
            switch (curr->bytes)
            {
            case 2:
                print_hex(program[i + 1]);
                break;
            case 3:
                print_hex(program[i + 2]);
                print_hex(program[i + 1]);
            default:
                break;
            }

            // TODO: print any suffixes, e.g. closing parens, offsets, etc.

            i += curr->bytes;
        }
        else
        {
            print("BAD: ");
            print_hex(program[i]);
            i++;
        }

        print("\r\n");
    }
}

void screen_clear()
{
    print("\033[2J\033[H");
}

void edit(loader_t *loader)
{
    bool exit = false;

    char *file_buffer = (char *)FILE_RAM;
    word file_index = 0;

    char in = 0;

    screen_clear();

    while (!exit)
    {
        in = getc();

        switch (in)
        {
        case 0x03: // Ctrl-C
            exit = true;
            break;
        case 0x0E: // Ctrl-N
            screen_clear();
            memset(file_buffer, 0, 1024);
            file_index = 0;
            break;
        case 0x7F: // Backspace
            putc(0x08);
            file_index--;
            file_buffer[file_index] = 0;
            break;
        case 0x0D: // Enter
            print("\r\n");
            file_buffer[file_index] = '\r';
            file_buffer[file_index + 1] = '\n';
            file_index += 2;
            break;
        default:
            putc(in);
            file_buffer[file_index] = in;
            file_index++;
            break;
        }
    }

    screen_clear();
}

void cat(loader_t *loader)
{
    print((char *)FILE_RAM);
}

void as(loader_t *loader)
{
    loader->binary_size = assemble((char *)FILE_RAM, (byte *)PROGRAM_RAM);
}
