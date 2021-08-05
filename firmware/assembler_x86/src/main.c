#include <stdio.h>
#include <stdlib.h>
#include "herring.h"
#include "assembler.h"

static char source[] = "lda #$FF\r\nsta $C002\r\nsta $C003\r\n";

void print(char *s)
{
    printf("%s", s);
}

void print_hex(word w)
{
    printf("%02X", w);
}

int main()
{
    byte machine_code[1024] = {0};

    word size = assemble(source, machine_code);

    for (int i = 0; i < size; i++)
    {
        print_hex(machine_code[i]);
        printf(" ");
    }

    printf("\r\n");

    return 0;
}