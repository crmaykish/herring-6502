#include <peekpoke.h>

#include "ch376s.h"
#include "herring.h"

char getc1()
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return PEEK(ACIA1_DATA);
}

void putc1(uint8_t c)
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_TX) == 0)
    {
    }

    POKE(ACIA1_DATA, c);
}

void puts1(const uint8_t *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        putc1(s[i]);
        ++i;
    }
}

void ch376s_send_command(uint8_t command)
{
    putc1(0x57);
    putc1(0xAB);
    putc1(command);
}

void ch376s_send_byte(uint8_t b)
{
    putc1(b);
}

void ch376s_send_string(char *s)
{
    puts1(s);
    putc1(0x00);
}

uint8_t ch376s_get_byte()
{
    return getc1();
}

bool ch376s_has_interrupt()
{
    return false;
}

bool ch376s_is_busy()
{
    return false;
}