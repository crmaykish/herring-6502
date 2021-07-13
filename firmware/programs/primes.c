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
    unsigned long count = 2;
    char prime_string[11];

    ACIA_Init();

    ACIA_WriteBuffer("prime numbers:");
    ACIA_NewLine();

    // Cheat and skip calculating two
    ACIA_WriteBuffer("1: 2");
    ACIA_NewLine();

    while (1)
    {
        if ((isPrime(prime)))
        {
            ultoa(count, prime_string, 10);
            ACIA_WriteBuffer(prime_string);
            ACIA_WriteBuffer(": ");
            ultoa(prime, prime_string, 10);
            ACIA_WriteBuffer(prime_string);
            ACIA_NewLine();

            count++;
        }

        prime += 2;
    }

    return 0;
}