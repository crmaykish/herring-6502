#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdbool.h>
#include "herring.h"

#define MNEMONIC_LENGTH 3
#define MAX_LABELS 32
#define MAX_LABEL_LENGTH 8

typedef enum
{
    ACC,   // Accumulator
    ABS,   // Absolute
    ABS_X, // Absolute, X-indexed
    ABS_Y, // Absolute, Y-indexed,
    IMM,   // Immediate (i.e #)
    IMPL,  // Implied
    IND,   // Indirect
    X_IND, // X-indexed, indirect
    IND_Y, // Indirect, Y-indexed
    REL,   // Relative
    ZP,    // Zeropage
    ZP_X,  // Zeropage, X-indexed
    ZP_Y   // Zeropage, Y-indexed
} address_mode_e;

typedef struct
{
    char mnemonic[4];
    uint8_t code;
    uint8_t bytes;
    address_mode_e mode;

} op_code_t;

typedef struct
{
    char labels[MAX_LABELS][MAX_LABEL_LENGTH + 1];
    uint16_t addresses[MAX_LABELS];
    uint8_t label_count;
} label_table_t;

// uint16_t assemble(char *source, uint8_t *dest);

// Brute-force lookup to go from mnemonic to opcode
op_code_t *mnemonic_to_opcode(char *mnemonic, uint16_t *operand, bool replace_labels);

// Look up full opcode details based on the hex value of the opcode
op_code_t *opcode_lookup(uint8_t op);

#endif
