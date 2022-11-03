#include <stdlib.h>
#include <stdio.h>
#include <peekpoke.h>
#include "serial.h"
#include "herring.h"

void serial_init()
{
    POKE(UART_LCR, 0x80);
    POKE(UART_DLL_LSB, 0x01); // Set baudrate to OSC / 16 / 1 = 115200
    POKE(UART_DLL_MSB, 0x00); // Set 0 to upper byte of baudrate clock divider
    POKE(UART_LCR, 0x03);     // 8 data bits, 1 stop bit, no parity, disable baud rate latch
}

bool serial_byte_available()
{
    return (PEEK(UART_LSR) & UART_RX_READY_FLAG);
}

char serial_getc()
{
    while ((PEEK(UART_LSR) & UART_RX_READY_FLAG) == 0)
    {
    }

    return PEEK(UART_RHR);
}

void serial_putc(uint8_t c)
{
    while ((PEEK(UART_LSR) & UART_TX_READY_FLAG) == 0)
    {
    }

    POKE(UART_THR, c);
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
