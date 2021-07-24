#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "basic.h"

// TODO: Define some constant values to interesting areas of memory that are accessible through BASIC
// Environment variables basically
// rom_start, ram_start, BASIC program memory start, IO device shortcuts

// BASIC "Memory"
static char program[BASIC_MAX_LINES][BASIC_LINE_LENGTH];

// BASIC "Registers"
static unsigned char program_counter = 0; // i.e. current line number

static char line_buffer[BASIC_LINE_LENGTH];

static print_f print_function;

static bool running = false;

void BASIC_Init(print_f pf)
{
    // Assign the print function pointer
    print_function = pf;

    BASIC_Erase();
}

void BASIC_List()
{
    // Print the program code
    unsigned char i = 0;
    for (i; i < BASIC_MAX_LINES; i++)
    {
        if (program[i][0] != 0)
        {
            sprintf(line_buffer, "%02d ", i);
            print_function(program[i]);
            print_function("\n\r");
        }
    }
}

// Completely erase the program code
void BASIC_Erase()
{
    memset(line_buffer, 0, BASIC_LINE_LENGTH);
    memset(program, 0, BASIC_MAX_LINES * BASIC_LINE_LENGTH);
}

// Store a command to the program memory at line
void BASIC_Store(unsigned char line_num, char *command)
{
    strncpy(program[line_num], command, BASIC_LINE_LENGTH);
}

void BASIC_ResetPC()
{
    program_counter = 0;
}

bool BASIC_Cycle()
{
    bool result = true;
    unsigned char original_pc = program_counter;

    if (program_counter == 255)
    {
        print_function("Last line in program\r\n");
        running = false;
    }

    if (program[program_counter][0] != 0)
    {
        // If there's an instruction at the program counter, try to run it
        result = BASIC_Interpret(program[program_counter]);
        running = result;
    }

    // If the program counter did not change after that instruction, increment it by 1
    if (original_pc == program_counter)
    {
        program_counter++;
    }

    return result;
}

bool BASIC_Interpret(char *command)
{
    char *instruction;
    char *param1;
    char *param2;
    // char *param3;

    // Copy command to line_buffer. Since strok messes with the strings, we don't want to change program code
    strncpy(line_buffer, command, BASIC_LINE_LENGTH);

    // Get the instruction portion of the command (everything up to the first delimiter)
    instruction = strtok(line_buffer, " +-*/=");

    if (strncmp(instruction, "print", 5) == 0)
    {
        // Print anything between double quotes

        // TODO: Perform variable substitution on the params

        param1 = strtok(NULL, "\""); // string to print

        print_function(param1);
        print_function("\n\r");
    }
    else if (strncmp(instruction, "goto", 4) == 0)
    {
        param1 = strtok(NULL, " "); // line number
        program_counter = (unsigned char)atoi(param1);
    }
    else if (strncmp(instruction, "end", 3) == 0)
    {
        return false;
    }
    else if (strncmp(instruction, "peek", 4) == 0)
    {
        unsigned short addr;
        unsigned char data;

        param1 = strtok(NULL, " "); // memory address in hex

        addr = (unsigned short)strtol(param1, NULL, 16);
        // data = PEEK(addr);

        sprintf(line_buffer, "%02X\n\r", data);
        print_function(line_buffer);
    }
    else if (strncmp(instruction, "poke", 4) == 0)
    {
        unsigned short addr;
        unsigned char data;

        param1 = strtok(NULL, " "); // memory address in hex
        param2 = strtok(NULL, " "); // data value in hex

        addr = (unsigned short)strtol(param1, NULL, 16);
        data = (unsigned char)strtol(param2, NULL, 16);

        // POKE(addr, data);
    }
    else
    {
        // TODO: Is the command a variable assignment?

        return false;
    }

    return true;
}

void BASIC_Parse(char * command)
{
    unsigned char line_number = 0;

    if (command[0] >= '0' && command[0] <= '9')
    {
        // Store a line of BASIC
        line_number = (unsigned char)strtol(command, NULL, 10);
        BASIC_Store(line_number, command + 2);
    }
    else if (strncmp(command, "list", 4) == 0)
    {
        BASIC_List();
    }
    else if (strncmp(command, "erase", 5) == 0)
    {
        BASIC_Erase();
    }
    else if (strncmp(command, "run", 3) == 0)
    {
        BASIC_ResetPC();
        running = true;
    }
    else if (strncmp(command, "move", 4) == 0)
    {
        // TODO: move a command to a new line
    }
    else
    {
        // Try to interpret the command as a BASIC instruction
        if (!BASIC_Interpret(command))
        {
            print_function("Syntax error\r\n");
        }
    }
}

unsigned char BASIC_IsRunning(){
    return running;
}