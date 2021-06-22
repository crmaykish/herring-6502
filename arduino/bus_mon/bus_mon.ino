/**
 * @file bus_mon.ino
 * @author Colin Maykish
 * @brief Serial Monitor for the Herring 6502 computer system
 * Reads the data and address buses and controls the clock and reset pins for the 6502
 * 
 * @date 2021-06-11
 * 
 * @copyright Copyright (c) 2021
 */

#include "bus_mon.h"
#include "cpu_6502.h"

CPU_6502_t cpu = {0};
char log_message[64] = {0};

void setup()
{
    // Start serial monitor
    Serial.begin(SERIAL_BAUDRATE);

    cpu_6502_init(&cpu, CLK_OUT, RST_OUT);
    cpu_6502_reset(&cpu);
    cpu_6502_run(&cpu);
}

void loop()
{
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');

        if (command.equals("RUN"))
        {
            log("Run");
            cpu_6502_run(&cpu);
        }
        else if (command.equals("STOP"))
        {
            log("Stop");
            cpu_6502_stop(&cpu);
        }
        else if (command.equals("RESET"))
        {
            cpu_6502_reset(&cpu);
        }
        else if (command.equals("SLOW"))
        {
            cpu.ClockSpeed = 40;
            log("Clock set to 40Hz");
        }
        else if (command.equals("MEDIUM"))
        {
            cpu.ClockSpeed = 500;
            log("Clock set to 500Hz");
        }
        else if (command.equals("FAST"))
        {
            cpu.ClockSpeed = 20000;
            log("Clock set to 20kHz");
        }
        else if (command.equals("TURBO"))
        {
            cpu.ClockSpeed = 100000;
            log("Clock set to 100kHz");
        }
        else if (command.equals("MAX"))
        {
            cpu.ClockSpeed = 500000;
            log("Clock set to 500kHz");
        }
        else if (!cpu.Running)
        {
            cpu_6502_cycle(&cpu, 1);

            // Print the buses after each cycle when single-stepping
            cpu_6502_dump(&cpu, log_message);
        }
    }

    if (cpu.Running)
    {
        cpu_6502_cycle(&cpu, 0);

        if (cpu.AddressBus != cpu.PrevAddressBus ||
            cpu.DataBus != cpu.PrevDataBus ||
            cpu.ControlFlags != cpu.PrevControlFlags)
        {
            // Only print the buses if something has changed
            cpu_6502_dump(&cpu, log_message);
        }
    }
}

void log(String s)
{
    Serial.println(s);
}

uint8_t reverse_bits(uint8_t b)
{
    return ((b & 0b1) << 7) |
           ((b & 0b10) << 5) |
           ((b & 0b100) << 3) |
           ((b & 0b1000) << 1) |
           ((b & 0b10000) >> 1) |
           ((b & 0b100000) >> 3) |
           ((b & 0b1000000) >> 5) |
           ((b & 0b10000000) >> 7);
}