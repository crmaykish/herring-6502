#include "herring.h"
#include "acia.h"
#include "terminal.h"
#include "print.h"

int main()
{
    print_line("Herring 6502");
    term_set_color(FG_COLOR_RED);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_GREEN);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_YELLOW);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BLUE);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_MAGENTA);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_CYAN);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_RED);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_GREEN);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_YELLOW);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_BLUE);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_MAGENTA);
    print_line("Herring 6502");
    term_set_color(FG_COLOR_BRIGHT_CYAN);
    print_line("Herring 6502");

    term_set_color(DEFAULT_COLORS);

    return 0;
}