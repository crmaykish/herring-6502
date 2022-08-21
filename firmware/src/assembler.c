#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serial.h"
#include "opcodes_6502.h"

// static label_table_t label_table;

uint8_t readline(char *buffer)
{
    uint8_t count = 0;
    uint8_t in = serial_getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            serial_putc(in);

            buffer[count] = in;
            count++;
        }

        in = serial_getc();
    }

    buffer[count] = 0;

    return count;
}

void main()
{
    char input[32] = {0};
    op_code_t *op;

    printf("Assembler\r\n");

    while (1)
    {
        readline(input);

        printf("\r\nGot: %s\r\n", input);

        op = mnemonic_to_opcode(input, 0, false);

        if (op != NULL)
        {
            printf("%s\r\n", op->mnemonic);
        }
        else
        {
            printf("NULL\r\n");
        }
    }
}

// void add_label(char *line, uint16_t addr)
// {
//     // TODO: if the label does not contain a ':', this will segfault

//     // printf("LABEL PARSE: %s\r\n", line);

//     strncpy(label_table.labels[label_table.label_count], line, (char *)strchr(line, ':') - (char *)line);
//     label_table.addresses[label_table.label_count] = addr;

//     // printf("Adding label %s: 0x%04X\r\n", label_table.labels[label_table.label_count], label_table.addresses[label_table.label_count]);

//     label_table.label_count++;
// }

// uint16_t get_label_val(char *line)
// {
//     uint8_t i = 0;

//     for (i; i < label_table.label_count; i++)
//     {
//         if (strcmp(label_table.labels[i], line) == 0)
//         {
//             return label_table.addresses[i];
//         }
//     }

//     return 0;
// }

// uint16_t assemble(char *source, uint8_t *dest)
// {
//     op_code_t *opcode = NULL;
//     uint16_t operand = 0;
//     uint16_t offset = 0;

//     char *source_copy = strdup(source);

//     char *line = strtok(source_copy, "\r\n");

//     // Reset label table
//     memset(label_table.labels, 0, MAX_LABELS * MAX_LABEL_LENGTH);
//     memset(label_table.addresses, 0, MAX_LABELS * sizeof(uint16_t));
//     label_table.label_count = 0;

//     // ==== PASS 1 ==== //
//     // Find all the labels and meta-instructions

//     // printf("\r\nStarting first pass...\r\n");

//     // TODO: cache the opcodes so we don't have to look them up in both passes. more ram, less cpu

//     while (line != NULL)
//     {
//         if (line[0] != ';')
//         {
//             opcode = mnemonic_to_opcode(line, &operand, false);

//             if (opcode == NULL)
//             {
//                 // printf("op code is NULL: %s\r\n", line);

//                 if (line[0] == '.')
//                 {
//                     if (strncmp(line, ".org", 4) == 0)
//                     {
//                         // Set offset to org operand
//                         offset = strtol(line + 6, NULL, 16);
//                     }
//                 }
//                 else
//                 {
//                     add_label(line, offset);
//                 }
//             }
//             else
//             {
//                 offset += opcode->uint8_ts;
//             }
//         }

//         line = strtok(NULL, "\r\n");
//     }

//     // ==== PASS 2 ==== //
//     // Translate instructions to opcodes

//     // printf("\r\nStarting second pass...\r\n");

//     // Reset all of the working variables before the second pass
//     free(source_copy);
//     source_copy = strdup(source);
//     line = strtok(source_copy, "\r\n");
//     opcode = NULL;
//     offset = 0;
//     operand = 0;

//     while (line != NULL)
//     {
//         if (line[0] != ';')
//         {
//             opcode = mnemonic_to_opcode(line, &operand, true);

//             if (opcode != NULL)
//             {
//                 // printf("%s => %02X, %04X\r\n", line, opcode->code, operand);

//                 dest[offset] = opcode->code;
//                 offset++;

//                 if (opcode->uint8_ts > 1)
//                 {
//                     dest[offset] = operand & 0xFF;
//                     offset++;
//                 }

//                 if (opcode->uint8_ts > 2)
//                 {
//                     dest[offset] = (operand & 0xFF00) >> 8;
//                     offset++;
//                 }
//             }
//         }

//         operand = 0;
//         opcode = NULL;

//         line = strtok(NULL, "\r\n");
//     }

//     free(source_copy);

//     return offset;
// }
