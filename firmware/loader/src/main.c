#include "herring.h"
#include "acia.h"

int main()
{
    char c = 32;

    acia_init();

    print("Herring 6502\r\n");

    while (true)
    {
        print("it's great!\r\n");

        // putc(c);

        // c++;

        // if (c >= 126)
        // {
        //     c = 32;
        // }
    }

    return 0;
}
