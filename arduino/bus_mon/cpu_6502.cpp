#include <Arduino.h>

#include "cpu_6502.h"
#include "bus_mon.h"

void cpu_6502_init(CPU_6502_t *cpu, unsigned int clock_pin, unsigned int reset_pin)
{
    cpu->ClockPin = clock_pin;
    cpu->ResetPin = reset_pin;
    cpu->ClockSpeed = DEFAULT_CLOCK_SPEED_HZ;

    pinMode(cpu->ClockPin, INPUT);
    pinMode(cpu->ResetPin, OUTPUT);

    // Set address, data, and control buses to inputs
    DDR_ADDR_HIGH |= 0x00;
    DDR_ADDR_LOW |= 0x00;
    DDR_DATA |= 0x00;
    DDR_CTRL |= 0x00;
}

void cpu_6502_cycle(CPU_6502_t *cpu, unsigned int count)
{
    for (int i = 0; i < count; i++)
    {
        digitalWrite(CLK_OUT, LOW);
        delay(10);
        digitalWrite(CLK_OUT, HIGH);
        delay(10);
    }

    cpu->PrevAddressBus = cpu->AddressBus;
    cpu->PrevDataBus = cpu->DataBus;
    cpu->PrevControlFlags = cpu->ControlFlags;

    // Read the CPU buses
    cpu->AddressBus = (reverse_bits(PIN_ADDR_HIGH) << 8) + PIN_ADDR_LOW;
    cpu->DataBus = PIN_DATA;
    cpu->ControlFlags = PIN_CTRL;
}

void cpu_6502_run(CPU_6502_t *cpu)
{
    tone(cpu->ClockPin, cpu->ClockSpeed);
    cpu->Running = true;
}

void cpu_6502_stop(CPU_6502_t *cpu)
{
    noTone(cpu->ClockPin);
    cpu->Running = false;
    digitalWrite(cpu->ClockPin, HIGH);
}

void cpu_6502_reset(CPU_6502_t *cpu)
{
    digitalWrite(cpu->ResetPin, LOW);
    delay(200);
    cpu_6502_cycle(cpu, 1);
    digitalWrite(cpu->ResetPin, HIGH);
    delay(200);
}

void cpu_6502_set_clock(CPU_6502_t *cpu, unsigned int clock_speed)
{
    cpu->ClockSpeed = clock_speed;

    if (cpu->Running)
    {
        noTone(cpu->ClockPin);
        tone(cpu->ClockPin, cpu->ClockSpeed);
    }
}

void cpu_6502_dump(CPU_6502_t *cpu, char *buffer)
{
    sprintf(buffer, "%04X | %02X | %c",
            cpu->AddressBus,
            cpu->DataBus,
//            (cpu->ControlFlags & (1 << 7)) >> 7,
//            (cpu->ControlFlags & (1 << 6)) >> 6,
//            (cpu->ControlFlags & (1 << 5)) >> 5,
//            (cpu->ControlFlags & (1 << 4)) >> 4,
//            (cpu->ControlFlags & (1 << 3)) >> 3,
//            (cpu->ControlFlags & (1 << 2)) >> 2,
//            (cpu->ControlFlags & (1 << 1)) >> 1,
            (cpu->ControlFlags & 1) ? 'R' : 'W');
}
