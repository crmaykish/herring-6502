#ifndef CPU_6502_H
#define CPU_6502_H

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    unsigned int ClockPin;
    unsigned int ResetPin;
    bool Running;
    uint16_t AddressBus;
    uint16_t PrevAddressBus;
    uint8_t DataBus;
    uint8_t PrevDataBus;
    uint8_t PrevControlFlags;
    uint8_t ControlFlags;
    unsigned int ClockSpeed;
} CPU_6502_t;

void cpu_6502_init(CPU_6502_t *cpu, unsigned int clock_pin, unsigned int reset_pin);
void cpu_6502_cycle(CPU_6502_t *cpu, unsigned int count);
void cpu_6502_run(CPU_6502_t *cpu);
void cpu_6502_stop(CPU_6502_t *cpu);
void cpu_6502_reset(CPU_6502_t *cpu);
void cpu_6502_set_clock(CPU_6502_t *cpu, unsigned int clock_speed);
void cpu_6502_dump(CPU_6502_t *cpu, char *buffer);

#endif