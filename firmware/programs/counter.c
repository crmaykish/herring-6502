#include "herring.h"
#include "via.h"

#define COUNTER_LIMIT 500

int main()
{
    word counter = 0;
    word display = 0b111;
    byte left = 1;

    VIA_InitAll();

    while (1)
    {
        if (counter >= COUNTER_LIMIT)
        {
            // timer expired
            counter = 0;

            poke(VIA0_PORTA, (display & 0xFF));
            poke(VIA0_PORTB, (display & 0xFF00) >> 8);

            if (display == 0b111000000000000)
            {
                left = 0;
            }
            else if (display == 0b111)
            {
                left = 1;
            }

            if (left)
            {
                display <<= 1;
            }
            else
                display >>= 1;
        }
        else
        {
            counter++;
        }
    }

    return 0;
}