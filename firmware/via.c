#include <peekpoke.h>
#include "via.h"
#include "herring.h"

void VIA_Init(byte ddra, byte ddrb)
{
    // Set the pin modes for each port
    POKE(VIA1_DDRA, ddra);
    POKE(VIA1_DDRB, ddrb);
}

void VIA_Write(word port, byte val)
{
    POKE(port, val);
}