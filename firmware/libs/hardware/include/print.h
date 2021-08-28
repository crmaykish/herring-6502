#ifndef PRINT_H
#define PRINT_H

#include "herring.h"

void print_hex(word w);
void print_dec(word w);
void print_string_bin(char *str, byte max);
void print_line(char *str);
void print_newline();

#endif