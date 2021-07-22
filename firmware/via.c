#include "via.h"
#include "herring.h"

void VIA_InitAll()
{
    poke(VIA0_DDRA, 0xFF);
    poke(VIA0_PORTA, 0x00);
    poke(VIA0_DDRB, 0xFF);
    poke(VIA0_PORTB, 0x00);

    poke(VIA1_DDRA, 0xFF);
    poke(VIA1_PORTA, 0x00);
    poke(VIA1_DDRB, 0xFF);
    poke(VIA1_PORTB, 0x00);
}