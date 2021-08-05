#include <stdio.h>
#include <stdlib.h>
#include "herring.h"
#include "assembler.h"

int main(int argc, char *argv[])
{
    char source[4096] = {0};
    byte machine_code[1024] = {0};

    FILE *source_file = fopen(argv[1], "r");
    FILE *bin_file = fopen(argv[2], "w+");
    printf("Reading source file...\r\n");

    int index = 0;

    char c = 0;

    while (c != EOF)
    {
        c = fgetc(source_file);
        source[index] = c;
        index++;
    }

    fclose(source_file);

    printf("Assembling source: \r\n");
    printf(source);

    word size = assemble(source, machine_code);

    printf("Machine code: \r\n");
    for (int i = 0; i < size; i++)
    {
        printf("%02X ", machine_code[i]);

        fputc(machine_code[i], bin_file);
    }

    printf("\r\n");

    fclose(bin_file);

    return 0;
}
