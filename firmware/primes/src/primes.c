#include <stdlib.h>
#include <stdbool.h>
#include <peekpoke.h>
#include "herring.h"
#include "lcd.h"

// very slow!
bool is_prime(word p)
{
    word w = 3;

    if (p % 2 == 0)
    {
        return false;
    }

    while (w < p / 2)
    {
        if (p % w == 0)
        {
            return false;
        }

        w += 2;
    }

    return true;
}

byte reverse_bits(byte b)
{
    return ((b & 0b1) << 7) |
           ((b & 0b10) << 5) |
           ((b & 0b100) << 3) |
           ((b & 0b1000) << 1) |
           ((b & 0b10000) >> 1) |
           ((b & 0b100000) >> 3) |
           ((b & 0b1000000) >> 5) |
           ((b & 0b10000000) >> 7);
}

int main()
{
    word count = 1;
    word prime = 2;
    char output_string[10] = {0};

    lcd_create();

    lcd_print("Primes:\r\n");

    while (true)
    {
        count = 1;
        prime = 2;

        while (count <= 5000)
        {
            if (is_prime(prime))
            {
                utoa(count, output_string, 10);
                lcd_print(output_string);
                lcd_print(": ");

                utoa(prime, output_string, 10);
                lcd_print(output_string);
                lcd_print("\r");

                POKE(0xC000, reverse_bits(prime & 0xFF));
                POKE(0xC001, reverse_bits((prime & 0xFF00) >> 8));

                count++;
            }

            prime++;
        }
    }

    return 0;
}