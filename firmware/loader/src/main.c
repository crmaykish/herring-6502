#include <stdbool.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

int main()
{
    char buffer[80];

    acia_init();

    print("\r\nHerring 6502\r\n");

    while (true)
    {
        print("> ");
        readline(buffer, true);

        print("\r\ngot: ");
        print(buffer);
        print("\r\n");
    }

    return 0;
}
