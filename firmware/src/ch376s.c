#include <peekpoke.h>

#include "ch376s.h"
#include "herring.h"

void ch376s_send_command(uint8_t command)
{
    POKE(CH376S_COMMAND, command);
}

void ch376s_send_byte(uint8_t b)
{
    POKE(CH376S_DATA, b);
}

void ch376s_send_string(char *s)
{
    uint8_t i = 0;

    while (s[i] != 0)
    {
        ch376s_send_byte((uint8_t)s[i]);
        i++;
    }

    // Always end a string by sending a zero
    ch376s_send_byte(0);
}

uint8_t ch376s_get_byte()
{
    return PEEK(CH376S_DATA);
}

bool ch376s_has_interrupt()
{
    return ((PEEK(CH376S_COMMAND) & CH376S_PARA_STATE_INTB) == 0);
}

bool ch376s_is_busy()
{
    return ((PEEK(CH376S_COMMAND) & CH376S_PARA_STATE_BZ) != 0);
}