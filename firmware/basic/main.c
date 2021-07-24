#include <stdio.h>
#include "basic.h"

void print_wrapper(char *buffer)
{
    printf("%s", buffer);
}

int main()
{
    char input[BASIC_LINE_LENGTH] = {0};
    int i;

    printf("Starting BASIC interpreter...\n\r");

    BASIC_Init(print_wrapper);

    while (1)
    {
        if (BASIC_IsRunning())
        {
            BASIC_Cycle();
        }
        else
        {
            printf("> ");
            fgets(input, BASIC_LINE_LENGTH, stdin);
            BASIC_Parse(input);
        }
    }

    return 0;
}