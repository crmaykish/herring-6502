#include <stdio.h>
#include <stdint.h>
#include "serial.h"

int main()
{
    printf("Hello, World! Here are some numbers: %d, %04X", 99, 0xBEEF);

    return 0;
}
