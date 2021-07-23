#include "herring.h"
#include "via.h"

#define COUNTER_LIMIT 500

int main()
{
    word counter = 0;
    word display = 0;

    VIA_InitAll();

    while (1)
    {
        if (counter >= COUNTER_LIMIT)
        {
            // timer expired
            counter = 0;

            poke(VIA0_PORTA, (display & 0xFF));
            poke(VIA0_PORTB, (display & 0xFF00) >> 8);
            display++;
        }
        else
        {
            counter++;
        }
    }

    return 0;
}