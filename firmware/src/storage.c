#include <stdio.h>
#include <stdint.h>
#include <peekpoke.h>

#include "herring.h"
#include "serial.h"
#include "ch376s.h"

uint8_t readline(char *buffer);

int main()
{
    char file_name[14];
    uint16_t file_size = 0;

    ch376s_init();

    ch376s_file_list();

    printf("Enter file name: ");

    readline(file_name);

    printf("\r\n");

    file_size = ch376s_file_read(file_name, (uint8_t *)0x5000, 0x8000 - 0x5000);
    printf("Read %d bytes\r\n", file_size);
    printf("File contents:\r\n%s\r\n", (uint8_t *)0x5000);

    printf("Exiting.\r\n");

    return 0;
}

uint8_t readline(char *buffer)
{
    uint8_t count = 0;
    uint8_t in = serial_getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            serial_putc(in);

            buffer[count] = in;
            count++;
        }

        in = serial_getc();
    }

    buffer[count] = 0;

    return count;
}
