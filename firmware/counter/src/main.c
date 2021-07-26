#include "via.h"

int main()
{
    VIA_OutputAll();

    VIA_SetPortA(0x55);

    while (true)
    {
    }

    return 0;
}
