#ifndef VIA_H
#define VIA_H

#include "herring.h"

void VIA_Init(byte ddra, byte ddrb);
void VIA_Write(word port, byte val);

#endif