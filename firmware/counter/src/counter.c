#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"

int main()
{
    word a = 0;

    while (true)
    {
        POKEW(0x8400, a);

        a++;
    }

    return 0;
}
