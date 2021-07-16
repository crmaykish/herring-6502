#include <stdlib.h>
#include "herring.h"
#include "acia.h"

bool isPrime(unsigned long p)
{
    unsigned long i = 2;

    for (i; i < p / 2; i++)
    {
        if (p % i == 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    unsigned long prime = 3;
    unsigned int count = 2;
    char prime_string[11];

    ACIA_Init(ACIA_TERM);

    ACIA_WriteBuffer(ACIA_TERM, "prime numbers:");
    ACIA_NewLine(ACIA_TERM);

    // Cheat and skip calculating two
    ACIA_WriteBuffer(ACIA_TERM, "1: 2");
    ACIA_NewLine(ACIA_TERM);

    while (1)
    {
        if ((isPrime(prime)))
        {
            utoa(count, prime_string, 10);
            ACIA_WriteBuffer(ACIA_TERM, prime_string);
            ACIA_WriteBuffer(ACIA_TERM, ": ");
            ultoa(prime, prime_string, 10);
            ACIA_WriteBuffer(ACIA_TERM, prime_string);
            ACIA_NewLine(ACIA_TERM);

            count++;
        }

        prime += 2;
    }

    return 0;
}