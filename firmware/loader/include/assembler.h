#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "herring.h"

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
    byte code;
    byte bytes;
    address_mode_e mode;

} op_code_t;

word assemble(char *source, byte *dest);

// Brute-force lookup to go from mnemonic to opcode
op_code_t *mnemonic_to_opcode(char *mnemonic, word *operand);

// Look up full opcode details based on the hex value of the opcode
op_code_t *opcode_lookup(byte op);

#endif
