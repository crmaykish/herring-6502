#include <stdlib.h>
#include <string.h>
// #include <stdio.h>
#include "assembler.h"

static label_table_t label_table;

static op_code_t opcodes[] = {
    // 0x00
    {"brk", 0x00, 1, IMPL},
    // {"---", 0x01, 1, ACC},
    // {"---", 0x02, 1, ACC},
    // {"---", 0x03, 1, ACC},
    // {"---", 0x04, 1, ACC},
    // {"---", 0x05, 1, ACC},
    {"asl", 0x06, 2, ZP},
    // {"---", 0x07, 1, ACC},
    {"php", 0x08, 1, IMPL},
    // {"---", 0x09, 1, ACC},
    {"asl", 0x0A, 1, ACC},
    // {"---", 0x0B, 1, ACC},
    // {"---", 0x0C, 1, ACC},
    // {"---", 0x0D, 1, ACC},
    {"asl", 0x0E, 3, ABS},
    // {"---", 0x0F, 1, ACC},
    // 0x10
    // {"---", 0x10, 1, ACC},
    // {"---", 0x11, 1, ACC},
    // {"---", 0x12, 1, ACC},
    // {"---", 0x13, 1, ACC},
    // {"---", 0x14, 1, ACC},
    // {"---", 0x15, 1, ACC},
    // {"---", 0x16, 1, ACC},
    // {"---", 0x17, 1, ACC},
    {"clc", 0x18, 1, IMPL},
    // {"---", 0x19, 1, ACC},
    // {"---", 0x1A, 1, ACC},
    // {"---", 0x1B, 1, ACC},
    // {"---", 0x1C, 1, ACC},
    // {"---", 0x1D, 1, ACC},
    // {"---", 0x1E, 1, ACC},
    // {"---", 0x1F, 1, ACC},
    // 0x20
    {"jsr", 0x20, 3, ABS},
    // {"---", 0x21, 1, ACC},
    // {"---", 0x22, 1, ACC},
    // {"---", 0x23, 1, ACC},
    // {"---", 0x24, 1, ACC},
    // {"---", 0x25, 1, ACC},
    {"rol", 0x26, 2, ZP},
    // {"---", 0x27, 1, ACC},
    {"plp", 0x28, 1, IMPL},
    {"and", 0x29, 2, IMM},
    {"rol", 0x2A, 1, ACC},
    // {"---", 0x2B, 1, ACC},
    // {"---", 0x2C, 1, ACC},
    {"and", 0x2D, 3, ABS},
    {"rol", 0x2E, 3, ABS},
    // {"---", 0x2F, 1, ACC},
    // 0x30
    // {"---", 0x30, 1, ACC},
    // {"---", 0x31, 1, ACC},
    // {"---", 0x32, 1, ACC},
    // {"---", 0x33, 1, ACC},
    // {"---", 0x34, 1, ACC},
    // {"---", 0x35, 1, ACC},
    // {"---", 0x36, 1, ACC},
    // {"---", 0x37, 1, ACC},
    // {"---", 0x38, 1, ACC},
    // {"---", 0x39, 1, ACC},
    // {"---", 0x3A, 1, ACC},
    // {"---", 0x3B, 1, ACC},
    // {"---", 0x3C, 1, ACC},
    // {"---", 0x3D, 1, ACC},
    // {"---", 0x3E, 1, ACC},
    // {"---", 0x3F, 1, ACC},
    // 0x40
    // {"---", 0x40, 1, ACC},
    // {"---", 0x41, 1, ACC},
    // {"---", 0x42, 1, ACC},
    // {"---", 0x43, 1, ACC},
    // {"---", 0x44, 1, ACC},
    // {"---", 0x45, 1, ACC},
    // {"---", 0x46, 1, ACC},
    // {"---", 0x47, 1, ACC},
    {"pha", 0x48, 1, IMPL},
    {"eor", 0x49, 2, IMM},
    // {"---", 0x4A, 1, ACC},
    // {"---", 0x4B, 1, ACC},
    {"jmp", 0x4C, 3, ABS},
    // {"---", 0x4D, 1, ACC},
    // {"---", 0x4E, 1, ACC},
    // {"---", 0x4F, 1, ACC},
    // 0x50
    // {"---", 0x50, 1, ACC},
    // {"---", 0x51, 1, ACC},
    // {"---", 0x52, 1, ACC},
    // {"---", 0x53, 1, ACC},
    // {"---", 0x54, 1, ACC},
    // {"---", 0x55, 1, ACC},
    // {"---", 0x56, 1, ACC},
    // {"---", 0x57, 1, ACC},
    // {"---", 0x58, 1, ACC},
    // {"---", 0x59, 1, ACC},
    // {"---", 0x5A, 1, ACC},
    // {"---", 0x5B, 1, ACC},
    // {"---", 0x5C, 1, ACC},
    // {"---", 0x5D, 1, ACC},
    // {"---", 0x5E, 1, ACC},
    // {"---", 0x5F, 1, ACC},
    // 0x60
    {"rts", 0x60, 1, IMPL},
    // {"---", 0x61, 1, ACC},
    // {"---", 0x62, 1, ACC},
    // {"---", 0x63, 1, ACC},
    // {"---", 0x64, 1, ACC},
    {"adc", 0x65, 2, ZP},
    {"ror", 0x66, 2, ZP},
    // {"---", 0x67, 1, ACC},
    {"pla", 0x68, 1, IMPL},
    {"adc", 0x69, 2, IMM},
    {"ror", 0x6A, 1, ACC},
    // {"---", 0x6B, 1, ACC},
    // {"---", 0x6C, 1, ACC},
    {"adc", 0x6D, 3, ABS},
    {"ror", 0x6E, 3, ABS},
    // {"---", 0x6F, 1, ACC},
    // 0x70
    // {"---", 0x70, 1, ACC},
    // {"---", 0x71, 1, ACC},
    // {"---", 0x72, 1, ACC},
    // {"---", 0x73, 1, ACC},
    // {"---", 0x74, 1, ACC},
    // {"---", 0x75, 1, ACC},
    // {"---", 0x76, 1, ACC},
    // {"---", 0x77, 1, ACC},
    {"sei", 0x78, 1, IMPL},
    // {"---", 0x79, 1, ACC},
    // {"---", 0x7A, 1, ACC},
    // {"---", 0x7B, 1, ACC},
    // {"---", 0x7C, 1, ACC},
    // {"---", 0x7D, 1, ACC},
    // {"---", 0x7E, 1, ACC},
    // {"---", 0x7F, 1, ACC},
    // 0x80
    // {"---", 0x80, 1, ACC},
    // {"---", 0x81, 1, ACC},
    // {"---", 0x82, 1, ACC},
    // {"---", 0x83, 1, ACC},
    // {"---", 0x84, 1, ACC},
    // {"---", 0x85, 1, ACC},
    // {"---", 0x86, 1, ACC},
    // {"---", 0x87, 1, ACC},
    {"dey", 0x88, 1, IMPL},
    // {"---", 0x89, 1, ACC},
    {"txa", 0x8A, 1, IMPL},
    // {"---", 0x8B, 1, ACC},
    // {"---", 0x8C, 1, ACC},
    {"sta", 0x8D, 3, ABS},
    // {"---", 0x8E, 1, ACC},
    // {"---", 0x8F, 1, ACC},
    // 0x90
    {"bcc", 0x90, 2, REL},
    // {"---", 0x91, 1, ACC},
    // {"---", 0x92, 1, ACC},
    // {"---", 0x93, 1, ACC},
    // {"---", 0x94, 1, ACC},
    // {"---", 0x95, 1, ACC},
    // {"---", 0x96, 1, ACC},
    // {"---", 0x97, 1, ACC},
    {"tya", 0x98, 1, IMPL},
    // {"---", 0x99, 1, ACC},
    {"txs", 0x9A, 1, IMPL},
    // {"---", 0x9B, 1, ACC},
    // {"---", 0x9C, 1, ACC},
    // {"---", 0x9D, 1, ACC},
    // {"---", 0x9E, 1, ACC},
    // {"---", 0x9F, 1, ACC},
    // 0xA0
    {"ldy", 0xA0, 2, IMM},
    // {"---", 0xA1, 1, ACC},
    {"ldx", 0xA2, 2, IMM},
    // {"---", 0xA3, 1, ACC},
    // {"---", 0xA4, 1, ACC},
    {"lda", 0xA5, 2, ZP},
    // {"---", 0xA6, 1, ACC},
    // {"---", 0xA7, 1, ACC},
    {"tay", 0xA8, 1, IMPL},
    {"lda", 0xA9, 2, IMM},
    {"tax", 0xAA, 1, IMPL},
    // {"---", 0xAB, 1, ACC},
    // {"---", 0xAC, 1, ACC},
    // {"---", 0xAD, 1, ACC},
    // {"---", 0xAE, 1, ACC},
    // {"---", 0xAF, 1, ACC},
    // 0xB0
    {"bcs", 0xB0, 2, REL},
    // {"---", 0xB1, 1, ACC},
    // {"---", 0xB2, 1, ACC},
    // {"---", 0xB3, 1, ACC},
    // {"---", 0xB4, 1, ACC},
    // {"---", 0xB5, 1, ACC},
    // {"---", 0xB6, 1, ACC},
    // {"---", 0xB7, 1, ACC},
    // {"---", 0xB8, 1, ACC},
    // {"---", 0xB9, 1, ACC},
    {"tsx", 0xBA, 1, IMPL},
    // {"---", 0xBB, 1, ACC},
    // {"---", 0xBC, 1, ACC},
    // {"---", 0xBD, 1, ACC},
    // {"---", 0xBE, 1, ACC},
    // {"---", 0xBF, 1, ACC},
    // 0xC0
    // {"---", 0xC0, 1, ACC},
    // {"---", 0xC1, 1, ACC},
    // {"---", 0xC2, 1, ACC},
    // {"---", 0xC3, 1, ACC},
    // {"---", 0xC4, 1, ACC},
    {"cmp", 0xC5, 2, ZP},
    // {"---", 0xC6, 1, ACC},
    // {"---", 0xC7, 1, ACC},
    {"iny", 0xC8, 1, IMPL},
    {"cmp", 0xC9, 2, IMM},
    {"dex", 0xCA, 1, IMPL},
    // {"---", 0xCB, 1, ACC},
    // {"---", 0xCC, 1, ACC},
    {"cmp", 0xCD, 3, ABS},
    // {"---", 0xCE, 1, ACC},
    // {"---", 0xCF, 1, ACC},
    // 0xD0
    {"bne", 0xD0, 2, REL},
    // {"---", 0xD1, 1, ACC},
    // {"---", 0xD2, 1, ACC},
    // {"---", 0xD3, 1, ACC},
    // {"---", 0xD4, 1, ACC},
    // {"---", 0xD5, 1, ACC},
    // {"---", 0xD6, 1, ACC},
    // {"---", 0xD7, 1, ACC},
    // {"---", 0xD8, 1, ACC},
    // {"---", 0xD9, 1, ACC},
    // {"---", 0xDA, 1, ACC},
    // {"---", 0xDB, 1, ACC},
    // {"---", 0xDC, 1, ACC},
    // {"---", 0xDD, 1, ACC},
    // {"---", 0xDE, 1, ACC},
    // {"---", 0xDF, 1, ACC},
    // 0xE0
    {"cpx", 0xE0, 2, IMM},
    // {"---", 0xE1, 1, ACC},
    // {"---", 0xE2, 1, ACC},
    // {"---", 0xE3, 1, ACC},
    {"cpx", 0xE4, 2, ZP},
    {"sbc", 0xE5, 2, ZP},
    {"inc", 0xE6, 2, ZP},
    // {"---", 0xE7, 1, ACC},
    {"inx", 0xE8, 1, IMPL},
    {"sbc", 0xE9, 2, IMM},
    {"nop", 0xEA, 1, IMPL},
    // {"---", 0xEB, 1, ACC},
    {"cpx", 0xEC, 3, ABS},
    {"sbc", 0xED, 3, ABS},
    {"inc", 0xEE, 3, ABS},
    // {"---", 0xEF, 1, ACC},
    // 0xF0
    {"beq", 0xF0, 2, REL},
    // {"---", 0xF1, 1, ACC},
    // {"---", 0xF2, 1, ACC},
    // {"---", 0xF3, 1, ACC},
    // {"---", 0xF4, 1, ACC},
    // {"---", 0xF5, 1, ACC},
    // {"---", 0xF6, 1, ACC},
    // {"---", 0xF7, 1, ACC},
    {"sed", 0xF8, 1, IMPL},
    // {"---", 0xF9, 1, ACC},
    // {"---", 0xFA, 1, ACC},
    // {"---", 0xFB, 1, ACC},
    // {"---", 0xFC, 1, ACC},
    // {"---", 0xFD, 1, ACC},
    // {"---", 0xFE, 1, ACC},
    // {"---", 0xFF, 1, ACC}
};

void add_label(char *line, word addr)
{
    // TODO: if the label does not contain a ':', this will segfault

    // printf("LABEL PARSE: %s\r\n", line);

    strncpy(label_table.labels[label_table.label_count], line, (char *)strchr(line, ':') - (char *)line);
    label_table.addresses[label_table.label_count] = addr;

    // printf("Adding label %s: 0x%04X\r\n", label_table.labels[label_table.label_count], label_table.addresses[label_table.label_count]);

    label_table.label_count++;
}

word get_label_val(char *line)
{
    byte i = 0;

    for (i; i < label_table.label_count; i++)
    {
        if (strcmp(label_table.labels[i], line) == 0)
        {
            return label_table.addresses[i];
        }
    }

    return 0;
}

word assemble(char *source, byte *dest)
{
    op_code_t *opcode = NULL;
    word operand = 0;
    word offset = 0;

    char *source_copy = strdup(source);

    char *line = strtok(source_copy, "\r\n");

    // Reset label table
    memset(label_table.labels, 0, MAX_LABELS * MAX_LABEL_LENGTH);
    memset(label_table.addresses, 0, MAX_LABELS * sizeof(word));
    label_table.label_count = 0;

    // ==== PASS 1 ==== //
    // Find all the labels and meta-instructions

    // printf("\r\nStarting first pass...\r\n");

    // TODO: cache the opcodes so we don't have to look them up in both passes. more ram, less cpu

    while (line != NULL)
    {
        if (line[0] != ';')
        {
            opcode = mnemonic_to_opcode(line, &operand, false);

            if (opcode == NULL)
            {
                // printf("op code is NULL: %s\r\n", line);

                if (line[0] == '.')
                {
                    // TODO: handle meta instructions
                    if (strncmp(line, ".org", 4) == 0)
                    {
                        // TODO: set offset to org operand
                    }
                }
                else
                {
                    add_label(line, offset);
                }
            }
            else
            {
                offset += opcode->bytes;
            }
        }

        line = strtok(NULL, "\r\n");
    }

    // ==== PASS 2 ==== //
    // Translate instructions to opcodes

    // printf("\r\nStarting second pass...\r\n");

    // Reset all of the working variables before the second pass
    free(source_copy);
    source_copy = strdup(source);
    line = strtok(source_copy, "\r\n");
    opcode = NULL;
    offset = 0;
    operand = 0;

    while (line != NULL)
    {
        if (line[0] != ';')
        {
            opcode = mnemonic_to_opcode(line, &operand, true);

            if (opcode != NULL)
            {
                // printf("%s => %02X, %04X\r\n", line, opcode->code, operand);

                dest[offset] = opcode->code;
                offset++;

                if (opcode->bytes > 1)
                {
                    dest[offset] = operand & 0xFF;
                    offset++;
                }

                if (opcode->bytes > 2)
                {
                    dest[offset] = (operand & 0xFF00) >> 8;
                    offset++;
                }
            }
        }

        operand = 0;
        opcode = NULL;

        line = strtok(NULL, "\r\n");
    }

    free(source_copy);

    return offset;
}

op_code_t *mnemonic_to_opcode(char *mnemonic, word *operand, bool replace_labels)
{
    byte i = 0;
    address_mode_e mode = IMPL;
    op_code_t *opcode = NULL;

    // 1) Find the addressing mode

    // TODO: This is not a complete addressing mode lookup scheme
    if (mnemonic[4] == '#')
    {
        mode = IMM;

        *operand = strtol(mnemonic + 6, NULL, 16);
    }
    else if (mnemonic[4] == '$')
    {
        mode = ABS;

        *operand = strtol(mnemonic + 5, NULL, 16);
    }
    // This is really fragile. Requires an instruction with no operands to have no space after it
    else if (mnemonic[3] == ' ' && mnemonic[4] >= 'a' && mnemonic[4] <= 'z')
    {
        mode = ABS;
    }

    // 2) Lookup the full opcode

    while (i < 0xFF)
    {
        if (strncmp(mnemonic, opcodes[i].mnemonic, 3) == 0 && mode == opcodes[i].mode)
        {
            opcode = &opcodes[i];
        }

        i++;
    }

    // 3) Check for labels in the operand and replace with their real value

    if (opcode != NULL)
    {
        if (replace_labels &&
            opcode->bytes == 3 &&
            mnemonic[4] >= 'a' &&
            mnemonic[4] <= 'z')
        {
            // operand is a label
            // Note: labels must be all lower-case letters
            *operand = get_label_val(&mnemonic[4]);
            // printf("Replace %s with $%04X\r\n", &mnemonic[4], *operand);
        }
    }

    return opcode;
}

op_code_t *opcode_lookup(byte op)
{
    byte i = 0;

    while (i < sizeof(opcodes) / sizeof(op_code_t))
    {
        if (opcodes[i].code == op)
        {
            return &opcodes[i];
        }

        i++;
    }

    return NULL;
}
