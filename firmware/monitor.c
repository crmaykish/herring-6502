#include <peekpoke.h>

#define VIA_PORTA 0x4001
#define VIA_PORTB 0x4000
#define VIA_DDRA 0x4003
#define VIA_DDRB 0x4002

static void VIA_Init()
{
    // Set all pins on Port B to outputs
    POKE(VIA_DDRB, 0xFF);
}

static void VIA_Write(char c)
{
    POKE(VIA_PORTB, c);
}

static unsigned char c = 0;

int main()
{
    VIA_Init();

    for (;;)
    {
        VIA_Write(0b111 << c);

        c++;

        if (c == 8)
        {
            c = 0;
        }
    }

    return 0;
}
