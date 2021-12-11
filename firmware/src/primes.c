#include <stdbool.h>
#include <stdint.h>
#include "herring.h"
#include "acia.h"
#include "print.h"

#define MAX_COUNT 100

bool is_prime(uint32_t p)
{
    uint32_t i;

    if (p < 2)
    {
        return false;
    }
    else if (p == 2)
    {
        return true;
    }
    else
    {
        for (i = 2; i < (p / 2) + 1; i++)
        {
            if (p % i == 0)
            {
                return false;
            }
        }
    }

    return true;
}

int main()
{
    uint16_t count = 1;
    uint32_t p = 0;

    print_line("Calculating primes...");

    while (count <= 100)
    {
        if (is_prime(p))
        {
            print_dec(count);
            print(": ");
            print_dec(p);
            print_newline();

            count++;
        }

        p++;
    }

    return 0;
}