#include <stdio.h>
#include <stdint.h>
#include <peekpoke.h>

#include "herring.h"
#include "ch376s.h"
#include "file_io.h"
#include "serial.h"
#include "delay.h"

#define CHECK_EXIST_REQUEST_VAL 0x55
#define CHECK_EXIST_RESPONSE_VAL 0xAA

uint8_t readline(char *buffer);

int main()
{
    char file_name[14];
    uint16_t file_size = 0;

    file_io_init();

    printf("Enter file name: ");

    readline(file_name);

    printf("\r\n");

    file_size = file_read(file_name, (uint8_t *)0x5000, 500);
    printf("Read %d bytes\r\n", file_size);
    printf("File contents: %s\r\n", (uint8_t *)0x5000);

    file_size = file_read_continue(file_name, (uint8_t *)0x6000, 600);
    printf("Read %d bytes\r\n", file_size);
    printf("File contents: %s\r\n", (uint8_t *)0x6000);

    file_size = file_read_continue(file_name, (uint8_t *)0x6000, 600);
    printf("Read %d bytes\r\n", file_size);
    printf("File contents: %s\r\n", (uint8_t *)0x6000);

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
