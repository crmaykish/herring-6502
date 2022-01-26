#include <stdio.h>
#include <stdint.h>

#include "serial.h"
#include "delay.h"

int main()
{
    printf("Hello, World! Here are some numbers: %d, %04X\r\n", 99, 0xBEEF);

    printf("Starting timer\r\n");

    while (1)
    {
        delay(10);
        serial_puts("tick\r\n");
    }

    return 0;
}
