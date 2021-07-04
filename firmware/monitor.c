#include "acia.h"

int main()
{
    ACIA_Init();

    for (;;)
    {
        ACIA_Write('C');
        ACIA_Write('o');
        ACIA_Write('l');
        ACIA_Write('i');
        ACIA_Write('n');
    }

    return 0;
}