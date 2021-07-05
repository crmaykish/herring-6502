#include "via.h"

int main()
{
    char c = 0;

    VIA_Init();

    for (;;)
    {
        VIA_Write(c);
        c++;
    }

    return 0;
}
