#include <stdbool.h>
#include "usb.h"
#include "herring.h"

static volatile uint8_t *data_port = (uint8_t *)USB_DATA_PORT;
static volatile uint8_t *command_port = (uint8_t *)USB_COMMAND_PORT;

void usb_send_command(uint8_t command)
{
    *command_port = command;
}

void usb_send_byte(uint8_t b)
{
    *data_port = b;
}

void usb_send_string(char *s)
{
    uint8_t i = 0;

    while (s[i] != 0)
    {
        usb_send_byte(s[i]);
        i++;
    }

    usb_send_byte(0);
}

uint8_t usb_get_byte()
{
    return *data_port;
}

void usb_wait_for_interrupt()
{
    // TODO: this needs a timeout

    while ((*command_port & 0b10000000) != 0)
    {
    }
}
