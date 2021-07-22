#ifndef UTILS_H
#define UTILS_H

#include "herring.h"

void PrintInt(word w);

// convert a single hex character to its integer value, i.e. 'A' => 10
byte HexToInt(char hex);

// convert a hex string to an int, e.g. "2000" => 4096
word IntegerValue(char *buffer);

void Delay(word w);

#endif