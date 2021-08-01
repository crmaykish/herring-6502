#include <stdlib.h>
#include <string.h>
#include "herring.h"
#include "acia.h"

void disassemble(byte *program, word size);
void memdump(word addr, byte rows);
void print_hex(word w);

int main()
{
    bool running = true;
    byte input[40] = {0};

    print("ASM65\r\n");

    while (running)
    {
        print("$ ");
        readline(input, true);
        print("\r\n");

        if (strncmp(input, "memdump", 7) == 0)
        {
            memdump(0x1000, 8);
        }
        else if (strncmp(input, "exit", 4) == 0)
        {
            running = false;
        }
        else if (strncmp(input, "da", 2) == 0)
        {
            disassemble((void *)0x1000, 100);
        }

        print("\r\n");
    }

    print("Exiting.\r\n");

    return 0;
}

void print_hex(word w)
{
    char s[7] = {0};
    utoa(w, s, 16);

    if (w < 0x10)
    {
        putc('0');
    }

    print(s);
}

void memdump(word addr, byte rows)
{
    byte i = 0;
    byte j = 0;
    byte m = 0;

    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < 16; i++)
        {
            m = PEEK(addr + (16 * j) + i);
            print_hex(m);
            putc(' ');

            if (i == 7)
            {
                putc(' ');
            }
        }

        print("\r\n");
    }
}

void disassemble(byte *program, word size)
{
    word i = 0;
    byte bytes = 0;

    while (i < size)
    {
        print_hex((word)program + i);
        print(": ");

        switch (program[i])
        {
        case 0xA9: // LDA #
            print("lda #$");
            print_hex(program[i + 1]);
            bytes = 2;
            break;

        case 0xA5: // LDA zp
            print("lda $");
            print_hex(program[i + 1]);
            bytes = 2;
            break;

        case 0xA2: // LDX #
            print("ldx #$");
            print_hex(program[i + 1]);
            bytes = 2;
            break;

        case 0xA0: // LDY #
            print("ldy #$");
            print_hex(program[i + 1]);
            bytes = 2;
            break;

        case 0x8D: // STA abs
            print("sta $");
            print_hex(program[i + 2]);
            print_hex(program[i + 1]);
            bytes = 3;
            break;

        case 0x4C:  // JMP abs
            print("jmp $");
            print_hex(program[i + 2]);
            print_hex(program[i + 1]);
            bytes = 3;
            break;
        case 0x20: // JSR
            print("jsr $");
            print_hex(program[i + 2]);
            print_hex(program[i + 1]);
            bytes = 3;
            break;

        case 0x60: // RTS
            print("rts");
            bytes = 1;
            break;
        default:
            print("XX");
            bytes = 1;
            break;
        }

        print("\r\n");

        i += bytes;
    }
}
