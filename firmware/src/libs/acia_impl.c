#include "acia.h"
#include "terminal.h"

byte readline(char *buffer, bool echo)
{
    byte count = 0;
    byte in = acia_getc();

    while (in != '\n' && in != '\r')
    {
        if (in == 0x7F) // Backspace
        {
            if (count != 0)
            {
                // Move cursor back one character and clear the previous character
                count--;
                buffer[count] = '\0';

                cursor_move(CURSOR_LEFT, 1);
                acia_putc(' ');
                cursor_move(CURSOR_LEFT, 1);
            }
        }
        else
        {
            if (echo)
            {
                acia_putc(in);
            }

            buffer[count] = in;
            count++;
        }

        in = acia_getc();
    }

    buffer[count] = 0;

    return count;
}
