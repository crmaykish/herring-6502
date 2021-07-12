#include <stdbool.h>
#include "herring.h"

#define COUNTER_LIMIT 500

int main()
{
    word counter = 0;
    word display = 0;

    poke(VIA1_DDRA, 0xFF);
    poke(VIA1_DDRB, 0xFF);
    poke(VIA1_PORTA, 0x00);
    poke(VIA1_PORTB, 0x00);

    while (true)
    {
        if (counter >= COUNTER_LIMIT)
        {
            // timer expired
            counter = 0;

            poke(VIA1_PORTA, (display & 0xFF));
            poke(VIA1_PORTB, (display & 0xFF00) >> 8);
            display++;
        }
        else
        {
            counter++;
        }
    }

    return 0;
}