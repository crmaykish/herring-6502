#include "herring.h"
#include "serial.h"

#pragma warn(unused-param, push, off)
int write(int fd, const char *buf, unsigned count)
{
    int i = 0;
    for (i; i < count; i++)
    {
        serial_putc(buf[i]);
    }

    return i;
}
#pragma warn(unused-param, pop)
