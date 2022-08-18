#include "herring.h"
#include <peekpoke.h>
#include <string.h>
#include <stdlib.h>
#include "vga.h"

int main()
{
    int i;

    srand(77);

    fb_clear();

    while (1)
    {
        for (i = 0; i < FB_SIZE; i++)
        {
            POKE(VGA_ADDR + i, rand() % 8);
        }

        SWAP_BUFFERS();
    }

    return 0;
}
