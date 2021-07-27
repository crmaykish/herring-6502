#ifndef VIA_H
#define VIA_H

#include "herring.h"

extern void via_init();
extern void via_port_a_set(byte val);
extern void via_port_b_set(byte val);
extern byte via_port_a_get();
extern byte via_port_b_get();

void via_set_word(word w);

#endif
