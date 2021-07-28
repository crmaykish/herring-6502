#include "herring.h"
#include "acia.h"

int main()
{
    char c = 32;

    acia_init();

    while (true)
    {
        putc(c);

        c++;

        if (c >= 126)
        {
            c = 32;
        }
    }

    return 0;
}
