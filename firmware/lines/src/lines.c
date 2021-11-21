#include "acia.h"
#include "print.h"
#include "framebuffer.h"

int main()
{
    byte x1 = 10;
    byte y1 = 10;
    byte x2 = 90;
    byte y2 = 80;

    framebuffer_init();

    while (true)
    {
        framebuffer_clear();

        draw_line(x1, y1, x2, y2, FB_GREEN);

        draw_line(10, 50, 90, 49, FB_RED);

        framebuffer_swap();
    }

    return 0;
}