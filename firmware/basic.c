#include <stdlib.h>
#include <string.h>
#include "basic.h"

static byte program[BASIC_MAX_LINES][BASIC_LINE_LENGTH];
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

void BASIC_Run()
{
}
