#include <peekpoke.h>

int main()
{
    unsigned char i = 0;

    POKE(0xC002, 0xFF);
    POKE(0xC003, 0xFF);

    for (;;)
    {
        POKE(0xC000, i);
        i++;
    }

    return 0;
}
