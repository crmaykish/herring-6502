#include "herring.h"
#include "serial.h"
#include <peekpoke.h>
#include "vga.h"

#pragma warn(unused-param, push, off)
int write(int fd, const char *buf, unsigned count)
{
    int i = 0;
    for (i; i < count; i++)
    {
        serial_putc(buf[i]);
#ifdef VGA_TEXTMODE_ENABLE
        TEXTMODE_WRITE_CHAR(buf[i]);
#endif
    }

    return i;
}
#pragma warn(unused-param, pop)

void print_string_bin(char *str, uint8_t max)
{
    uint8_t i = 0;

    while (i < max)
    {
        if (str[i] >= 32 && str[i] < 127)
        {
            serial_putc(str[i]);
#ifdef VGA_TEXTMODE_ENABLE
            TEXTMODE_WRITE_CHAR(str[i]);
#endif
        }
        else
        {
            serial_putc('.');
#ifdef VGA_TEXTMODE_ENABLE
            TEXTMODE_WRITE_CHAR('.');
#endif
        }

        i++;
    }
}