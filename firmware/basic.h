#ifndef BASIC_H
#define BASIC_H

#include <stdbool.h>
#include "herring.h"

#define BASIC_MAX_LINES 0xFF
#define BASIC_LINE_LENGTH 40

// Function pointer that takes a single string and outputs it somewhere user-facing
typedef void (*print_f)(char *buffer);

void BASIC_Init(print_f pf);

// Output the program code by calling print_function() for each command
void BASIC_List();

// Completely erase the program code
void BASIC_Erase();

// Store a command to the program memory at line
void BASIC_Store(byte line_num, byte *command);

// Reset the program counter
void BASIC_ResetPC();

// Execute the BASIC instruction at the current program counter
// and set the program counter to the appropriate next instruction
bool BASIC_Cycle();

// Interpret a single BASIC instruction
bool BASIC_Interpret(byte *command);

#endif BASIC_H
