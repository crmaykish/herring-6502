#include <stdlib.h>
#include <string.h>
#include "basic.h"

// BASIC "Memory"
static byte program[BASIC_MAX_LINES][BASIC_LINE_LENGTH];

// BASIC "Registers"
static byte program_counter = 0; // i.e. current line number

static byte line_buffer[BASIC_LINE_LENGTH];

static print_f print_function;

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
            itoa(i, line_buffer, 10);
            print_function(line_buffer);
            print_function(" ");
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
void BASIC_Store(byte line_num, byte *command)
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
    byte original_pc = program_counter;

    if (program_counter == 255)
    {
        print_function("Last line in program\r\n");
        return false;
    }

    if (program[program_counter][0] != 0)
    {
        // If there's an instruction at the program counter, try to run it
        result = BASIC_Interpret(program[program_counter]);
    }

    // If the program counter did not change after that instruction, increment it by 1
    if (original_pc == program_counter)
    {
        program_counter++;
    }

    return result;
}

bool BASIC_Interpret(byte *command)
{
    char *instruction;
    char *param1;
    // char *param2;
    // char *param3;

    // Copy command to line_buffer. Since strok messes with the strings, we don't want to change program code
    strncpy(line_buffer, command, BASIC_LINE_LENGTH);

    // Get the instruction portion of the command (everything up to the first delimiter)
    instruction = strtok(line_buffer, " +-*/=");

    if (strcmp(instruction, "print") == 0)
    {
        // Print anything between double quotes

        // TODO: Perform variable substitution on the params

        param1 = strtok(NULL, "\"");

        print_function(param1);
        print_function("\n\r");
    }
    else if (strcmp(instruction, "goto") == 0)
    {
        param1 = strtok(NULL, " ");
        program_counter = (byte)atoi(param1);
    }
    else if (strcmp(instruction, "end") == 0)
    {
        return false;
    }
    else
    {
        // TODO: Is the command a variable assignment?
    }

    return true;
}
