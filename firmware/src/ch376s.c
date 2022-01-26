#include "ch376s.h"
#include "herring.h"

// Pointers to the memory-mapped registers of the CH376S module
static volatile uint8_t *data_port = (uint8_t *)USB_DATA_PORT;
static volatile uint8_t *command_port = (uint8_t *)USB_COMMAND_PORT;

void ch376s_send_command(uint8_t command)
{
    *command_port = command;
}

void ch376s_send_byte(uint8_t b)
{
    *data_port = b;
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
    return *data_port;
}

bool ch376s_has_interrupt()
{
    return ((*command_port & CH376S_PARA_STATE_INTB) == 0);
}
