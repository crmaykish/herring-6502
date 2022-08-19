#include <stdlib.h>
#include <stdio.h>
#include <peekpoke.h>
#include "serial.h"
#include "herring.h"

void serial_init()
{
    POKE(ACIA0_STATUS, ACIA_RESET);
    POKE(ACIA0_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA0_CONTROL, ACIA_CONTROL_BAUD);
}

bool serial_byte_available()
{
    return (PEEK(ACIA0_STATUS) & ACIA_READY_RX);
}

char serial_getc()
{
    while ((PEEK(ACIA0_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return PEEK(ACIA0_DATA);
}

void serial_putc(uint8_t c)
{
    while ((PEEK(ACIA0_STATUS) & ACIA_READY_TX) == 0)
    {
    }

    POKE(ACIA0_DATA, c);

    if (c == 0x0A)
    {
        serial_putc(0x0D);
    }
}

void serial_puts(const uint8_t *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        serial_putc(s[i]);
        ++i;
    }
}

void term_set_color(char *color)
{
    serial_puts("\033[");
    serial_puts(color);
    serial_putc('m');
}

void term_cursor_move(term_cursor_dir_e dir, uint8_t steps)
{
    printf("\033[%d%c", steps, dir);
}

void term_cursor_set_x(uint8_t x)
{
    printf("\033[%dG", x);
}

void term_cursor_set_pos(uint8_t x, uint8_t y)
{
    printf("\033[%d;%dH", x, y);
}

void term_cursor_set_vis(bool visible)
{
    serial_puts(visible ? TERM_CURSOR_SHOW : TERM_CURSOR_HIDE);
}

void term_clear()
{
    term_set_color(TERM_RESET);
    serial_puts("\033[2J\033[H");
}
