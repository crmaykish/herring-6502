#include <stdlib.h>
#include <peekpoke.h>
#include "serial.h"
#include "herring.h"

void serial_init()
{
    POKE(ACIA0_STATUS, ACIA_RESET);
    POKE(ACIA0_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA0_CONTROL, ACIA_CONTROL_BAUD);
}

// TODO: implement this in a better way
// pass a pointer to a char to fill with data, return true or false if it read anything
// bool serial_rx_ready()
// {
//     return (PEEK(ACIA0_STATUS) & ACIA_READY_RX);
// }

char getc()
{
    while ((PEEK(ACIA0_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return PEEK(ACIA0_DATA);
}

void putc(uint8_t c)
{
    uint8_t delay = 0xFF; // TODO: this might be more delay than necessary

    while ((PEEK(ACIA0_STATUS) & ACIA_READY_TX) == 0)
    {
    }

    POKE(ACIA0_DATA, c);

    // Delay loop to work around the WDC65C51 hardware bug
    // See: http://forum.6502.org/viewtopic.php?f=4&t=2543&start=30
    while (delay > 0)
    {
        delay--;
    }
}

void puts(const uint8_t *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        putc(s[i]);
        ++i;
    }
}

void print_hex(uint16_t w)
{
    char s[5] = {0};
    utoa(w, s, 16);

    if (w < 0x10)
    {
        putc('0');
    }

    puts(s);
}

void print_dec(uint16_t w)
{
    char s[6] = {0};
    utoa(w, s, 10);
    puts(s);
}

void print_string_bin(char *str, uint8_t max)
{
    uint8_t i = 0;

    while (i < max)
    {
        if (str[i] >= 32 && str[i] < 127)
        {
            putc(str[i]);
        }
        else
        {
            putc('.');
        }

        i++;
    }
}

void print_line(char *str)
{
    if (str != 0)
    {
        puts(str);
    }

    putc(ASCII_NEWLINE);
    putc(ASCII_RETURN);
}

void term_set_color(char *color)
{
    puts("\033[");
    puts(color);
    putc('m');
}

void term_cursor_move(term_cursor_dir_e dir, uint8_t steps)
{
    char command[10];
    utoa(steps, command, 10);
    puts("\033[");
    puts(command);
    putc((char)dir);
}

void term_cursor_set_x(uint8_t x)
{
    puts("\033[");
    print_dec(x);
    putc('G');
}

void term_cursor_set_pos(uint8_t x, uint8_t y)
{
    puts("\033[");
    print_dec(y);
    putc(';');
    print_dec(x);
    putc('H');
}

void term_cursor_set_vis(bool visible)
{
    puts(visible ? TERM_CURSOR_SHOW : TERM_CURSOR_HIDE);
}

void term_clear()
{
    term_set_color(TERM_RESET);
    puts("\033[2J\033[H");
}
