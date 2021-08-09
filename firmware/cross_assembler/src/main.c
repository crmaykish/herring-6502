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

    char c = fgetc(source_file);

    while (c != EOF)
    {
        source[index] = c;
        index++;
        c = fgetc(source_file);
    }

    fclose(source_file);

    printf("Assembling source: \r\n");
    printf(source);
    printf("\r\n");

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
