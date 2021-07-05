#include "via.h"

static unsigned char c = 0;

int main()
{
    VIA_Init();

    for (;;)
    {
        VIA_Write(c);
        c++;
    }

    return 0;
}
