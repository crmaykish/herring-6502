#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "herring.h"
#include "acia.h"

bool isPrime(uint16_t p)
{
    uint16_t i = 2;

    for (i; i < p / 2; i++)
    {
        if (p % i == 0)
        {
            return false;
        }
    }

    return true;
}

// TODO: this is resetting at 1277, why?

int main()
{
    uint16_t prime = 3;
    char prime_string[11];

    ACIA_Init();

    ACIA_WriteBuffer("prime numbers:");
    ACIA_NewLine();

    while (true)
    {
        if ((isPrime(prime)))
        {
            ultoa(prime, prime_string, 10);
            ACIA_WriteBuffer(prime_string);
            ACIA_NewLine();
        }

        prime += 2;
    }

    return 0;
}