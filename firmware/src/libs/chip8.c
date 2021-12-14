#include "chip8.h"
#include <stdlib.h>
// #include <stdio.h>
#include <string.h>

void chip8_init(chip8_cpu_t *cpu, uint8_t (*delay_timer)(uint8_t))
{
    if (cpu == NULL)
    {
        DEBUG_PRINT("chip8_cpu_t pointer is NULL");
        return;
    }

    // Set the delay timer function pointer
    cpu->DelayTimer = delay_timer;

    // Start the delay timer at 0xFF
    cpu->DelayTimer(0xFF);

    // Initialize CPU state and memory
    cpu->I = 0;
    cpu->PC = PC_START;
    cpu->StackPointer = 0;
    memset(cpu->V, 0, REGISTER_COUNT);
    memset(cpu->Stack, 0, STACK_SIZE);
    // Clear the system memory
    memset(cpu->Memory, 0, MEMORY_SIZE);
    // Clear the screen memory
    memset(cpu->Screen, 0, CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT);
    // Load the font into system memory
    memcpy(cpu->Memory, FONT, sizeof(FONT));
}

void chip8_cycle(chip8_cpu_t *cpu)
{
    uint16_t opcode;
    uint16_t nnn;
    uint8_t n;
    uint8_t x, y;
    uint8_t kk;

    uint16_t temp;
    uint8_t i, j;

    uint8_t *sprite;

    uint8_t sx, sy;

    if (cpu == NULL)
    {
        DEBUG_PRINT("chip8_cpu_t pointer is NULL");
        return;
    }

    // Fetch opcode
    opcode = (cpu->Memory[cpu->PC] << 8) + cpu->Memory[cpu->PC + 1];

    nnn = opcode & 0x0FFF;
    n = opcode & 0x000F;
    x = (opcode & 0x0F00) >> 8;
    y = (opcode & 0x00F0) >> 4;
    kk = opcode & 0x00FF;

    switch ((opcode & 0xF000) >> 12)
    {
    case 0:
        switch (kk)
        {
        case 0xE0: // CLS
            DEBUG_PRINT("CLEAR SCREEN\r\n");
            cpu->PC += 2;
            break;
        case 0xEE: // RET
            cpu->StackPointer--;
            cpu->PC = cpu->Stack[cpu->StackPointer];
            break;
        default:
            DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
            exit(1);
            break;
        }

        break;

    case 1: // JP
        cpu->PC = nnn;
        break;

    case 2: // CALL
        cpu->Stack[cpu->StackPointer] = cpu->PC;
        cpu->StackPointer++;
        cpu->PC = nnn;
        break;
    case 3: // SE (skip if equal)
        cpu->PC += 2;

        if (cpu->V[x] == kk)
        {
            cpu->PC += 2;
        }

        break;
    case 4: // SNE (skip if not equal)
        cpu->PC += 2;

        if (cpu->V[x] != kk)
        {
            cpu->PC += 2;
        }

        break;
    case 5: // SE (skip if registers are equal)
        cpu->PC += 2;

        if (cpu->V[x] == cpu->V[y])
        {
            cpu->PC += 2;
        }

        break;
    case 6: // LD (load kk into Vx)
        cpu->PC += 2;
        cpu->V[x] = kk;
        break;
    case 7: // ADD (Vx += kk)
        cpu->PC += 2;
        cpu->V[x] += kk;
        break;
    case 8:

        switch (n)
        {
        case 0: // LD (Vx = Vy)
            cpu->PC += 2;
            cpu->V[x] = cpu->V[y];
            break;
        case 1: // OR (Vx |= Vy)
            cpu->PC += 2;
            cpu->V[x] |= cpu->V[y];
            break;
        case 2: // AND (Vx &= Vy)
            cpu->PC += 2;
            cpu->V[x] &= cpu->V[y];
            break;
        case 3: // XOR (Vx ^= Vy)
            cpu->PC += 2;
            cpu->V[x] ^= cpu->V[y];
            break;
        case 4: // ADD (Vx += Vy)
            cpu->PC += 2;
            temp = cpu->V[x] + cpu->V[y];
            cpu->V[x] = temp & 0xFF;
            cpu->V[0xF] = (temp > 255) ? 1 : 0;
            break;
        case 5: // SUB Vx, Vy
            cpu->PC += 2;

            cpu->V[x] -= cpu->V[y];

            if (cpu->V[x] > cpu->V[y])
            {
                cpu->V[0xF] = 1;
            }
            else
            {
                cpu->V[0xF] = 0;
            }

            break;
            // case 6:
            //     break;
            // case 7:
            //     break;
            // case 0xE:
            //     break;

        default:
            DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
            exit(1);
            break;
        }

        break;
    case 9: // SNE Vx, Vy

        cpu->PC += 2;

        if (cpu->V[x] != cpu->V[y])
        {
            cpu->PC += 2;
        }

        break;
    case 0xA: // LD I, nnn
        cpu->PC += 2;
        cpu->I = nnn;
        break;
    case 0xB:
        DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
        exit(1);
        break;
    case 0xC: // RND Vx, byte
        cpu->PC += 2;

        i = rand() % 0xFF;

        cpu->V[x] = i & kk;

        break;
    case 0xD:
        cpu->PC += 2;

        // TODO: this does not handle sprites wrapping off screen
        // TODO: this does not properly set Vf flag

        // Read n bytes from memory at I
        sprite = &cpu->Memory[cpu->I];

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < 8; j++)
            {
                sx = cpu->V[x] + j;
                sy = cpu->V[y] + i;

                if (sx < CHIP8_SCREEN_WIDTH && sy < CHIP8_SCREEN_HEIGHT)
                {
                    cpu->Screen[sx][sy] ^= sprite[i] & (1 << (7 - j));
                }
            }
        }

        break;
    case 0xE:

        switch (kk)
        {
        case 0x9E: // SKP Vx
            cpu->PC += 2;
            DEBUG_PRINT("TODO: SKP if key Vx is pressed\r\n");
            break;
        case 0xA1:
            cpu->PC += 2; // SKNP Vx
            DEBUG_PRINT("TODO: SKP if key Vx is NOT pressed\r\n");
            break;
            break;
        default:
            DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
            exit(1);
            break;
        }

        break;
    case 0xF:
        switch (kk)
        {
        case 0x07: // LD Vx, DT
            cpu->PC += 2;
            cpu->V[x] = cpu->DelayTimer(0);

            break;
        case 0x15: // LD DT, Vx
            cpu->PC += 2;
            cpu->DelayTimer(cpu->V[x]);

        case 0x1E: // ADD I, Vx
            cpu->PC += 2;
            cpu->I += cpu->V[x];
            break;

            break;
        case 0x29: // LD F, Vx
            cpu->PC += 2;
            // Set I = memory location where the sprite for value of Vx is

            cpu->I = cpu->V[x] * 5;

            break;
        case 0x33: //LD B, Vx
            cpu->PC += 2;
            cpu->Memory[cpu->I] = cpu->V[x] / 100;
            cpu->Memory[cpu->I + 1] = (cpu->V[x] / 100) % 10;
            cpu->Memory[cpu->I + 2] = (cpu->V[x] % 100) % 10;
            break;

        case 0x55:
            cpu->PC += 2;

            memcpy(&cpu->Memory[cpu->I], cpu->V, x + 1);

        case 0x65: // LD Vx, [I]
            cpu->PC += 2;

            // TODO: include x or not?
            memcpy(cpu->V, &cpu->Memory[cpu->I], x + 1);

            break;

        default:
            DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
            exit(1);
            break;
        }

        break;
    default:
        DEBUG_PRINT("NOT IMPLEMENTED: %04X\r\n", opcode);
        cpu->PC += 2;

        break;
    }
}
