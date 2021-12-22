#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>
#include <stdint.h>

// Terminal colors
#define TERM_RESET "0"
#define TERM_FG_RED "31"
#define TERM_FG_GREEN "32"
#define TERM_FG_YELLOW "33"
#define TERM_FG_BLUE "34"
#define TERM_FG_MAGENTA "35"
#define TERM_FG_CYAN "36"
#define TERM_FG_BRIGHT_RED "91"
#define TERM_FG_BRIGHT_GREEN "92"
#define TERM_FG_BRIGHT_YELLOW "93"
#define TERM_FG_BRIGHT_BLUE "94"
#define TERM_FG_BRIGHT_MAGENTA "95"
#define TERM_FG_BRIGHT_CYAN "96"

#define TERM_BG_RED "41"
#define TERM_BG_GREEN "42"
#define TERM_BG_YELLOW "43"
#define TERM_BG_BLUE "44"
#define TERM_BG_MAGENTA "45"
#define TERM_BG_CYAN "46"
#define TERM_BG_BRIGHT_RED "101"
#define TERM_BG_BRIGHT_GREEN "102"
#define TERM_BG_BRIGHT_YELLOW "103"
#define TERM_BG_BRIGHT_BLUE "104"
#define TERM_BG_BRIGHT_MAGENTA "105"
#define TERM_BG_BRIGHT_CYAN "106"

// Terminal cursor controls
#define TERM_CURSOR_HIDE "\033[?25l"
#define TERM_CURSOR_SHOW "\033[?25h"

typedef uint8_t term_cursor_dir_e;
#define TERM_CURSOR_UP 'A'
#define TERM_CURSOR_DOWN 'B'
#define TERM_CURSOR_RIGHT 'C'
#define TERM_CURSOR_LEFT 'D'

// === Serial port control === //
void serial_init();
bool serial_byte_available();

// === Low level serial I/O === //
char getc();
void putc(uint8_t);
void puts(const uint8_t *);

// === String formatting functions === //
void print_hex(uint16_t w);
void print_dec(uint16_t w);
void print_string_bin(char *str, uint8_t max);
void print_line(char *str);

// === Terminal font colors and control === //
void term_set_color(char *color);
void term_cursor_move(term_cursor_dir_e dir, uint8_t steps);
void term_cursor_set_x(uint8_t x);
void term_cursor_set_pos(uint8_t x, uint8_t y);
void term_cursor_set_vis(bool visible);
void term_clear();

#endif
