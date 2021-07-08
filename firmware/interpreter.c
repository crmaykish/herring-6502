#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "herring.h"
#include "acia.h"
#include "basic.h"

// Message output buffer
static byte output[SERIAL_BUFFER_SIZE]; // TODO: move this into the serial code
static char *command;

byte line_number = 0;
byte basic_running = 0;

void Parse_Command()
{
    if (command[0] >= '0' && command[0] <= '9')
    {
        // Store a line of BASIC
        line_number = (byte)strtol(command, NULL, 10);
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
        basic_running = 1;
    }
    else
    {
        // TODO: try to interpret the command as a BASIC instruction

        // If that fails...
        ACIA_WriteLine("Syntax error");
    }
}

void main()
{
    command = ACIA_GetBuffer();

    ACIA_Init();
    BASIC_Init(ACIA_WriteBuffer);
    ACIA_WriteLine("Herring BASIC ><(((°>");

    // Main application loop
    for (;;)
    {
        // Show the prompt
        ACIA_WriteBuffer("> ");

        // Block until the user enters a command
        ACIA_ReadLine();
        ACIA_NewLine();

        Parse_Command();

        // Clear the serial buffer and output buffer
        ACIA_ClearBuffer();
        memset(output, 0, SERIAL_BUFFER_SIZE);
        memset(command, 0, SERIAL_BUFFER_SIZE);
    }
}
