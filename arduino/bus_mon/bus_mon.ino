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

#include <SPI.h>
#include <SD.h>
#include "bus_mon.h"
#include "cpu_6502.h"

bool sd_connected = false;

CPU_6502_t cpu = {0};
char log_message[256] = {0};

File sd_root;
File curr;

void list_files()
{
    if (!sd_connected)
    {
        log("No SD card");
        return;
    }

    sd_root = SD.open("/");
    curr = sd_root.openNextFile();

    while (curr)
    {
        Serial.println(curr.name());
        // Serial.println(curr.size());
        curr = sd_root.openNextFile();
    }

    sd_root.close();
    curr.close();
}

void hexdump(String filename)
{
    uint8_t file[16];

    if (!sd_connected)
    {
        log("No SD card");
        return;
    }

    curr = SD.open(filename);

    if (!curr)
    {
        log("No such file");
        return;
    }

    uint16_t total_bytes = 0;

    while (curr.available())
    {
        int b = curr.readBytes(file, 16);

        Serial.print(total_bytes, HEX);
        Serial.print("  ");

        total_bytes += b;

        for (int i = 0; i < b; i++)
        {
            Serial.print(file[i], HEX);
            if (i == 7)
            {
                Serial.print("  ");
            }
            else
            {
                Serial.print(" ");
            }
        }

        Serial.print("  |");

        for (int i = 0; i < b; i++)
        {
            if (file[i] >= 32 && file[i] < 127)
            {
                Serial.write(file[i]);
            }
            else
            {
                Serial.print(".");
            }
        }
        Serial.println("|");
    }

    curr.close();
}

uint8_t read_ram(uint16_t addr)
{
    // Set data bus to input
    DDR_DATA = 0x00;

    //Set address bus to output
    DDR_ADDR_HIGH = 0xFF;
    DDR_ADDR_LOW = 0xFF;

    delay(10);

    PORT_ADDR_LOW = (addr & 0xFF);
    PORT_ADDR_HIGH = reverse_bits((addr & 0xFF00) >> 8);

    delay(10);
    return PIN_DATA;
}

void write_ram(uint16_t addr, uint8_t val)
{
    // Set data bus to output
    DDR_DATA = 0xFF;

    //Set address bus to output
    DDR_ADDR_HIGH = 0xFF;
    DDR_ADDR_LOW = 0xFF;

    // Set data bus
    PORT_DATA = val;

    delay(20);

    digitalWrite(CLK_OUT, HIGH);
    digitalWrite(RWB_OUT, LOW);
    delay(20);

    // Set address bus
    PORT_ADDR_LOW = (addr & 0xFF);
    PORT_ADDR_HIGH = reverse_bits((addr & 0xFF00) >> 8);

    delay(20);

    digitalWrite(RWB_OUT, HIGH);
    delay(10);
}

void setup()
{
    // RW set to HIGH (read)
    pinMode(RWB_OUT, OUTPUT);
    digitalWrite(RWB_OUT, HIGH);

    // BUS ENABLE OFF
    pinMode(BE_OUT, OUTPUT);
    digitalWrite(BE_OUT, LOW);

    // Start serial monitor
    Serial.begin(SERIAL_BAUDRATE);

    sd_connected = SD.begin(SD_SELECT);

    cpu_6502_init(&cpu, CLK_OUT, RST_OUT);
    cpu_6502_reset(&cpu);
    // cpu_6502_run(&cpu);

    log("Monitor Ready.");
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
            log("Reset");
            cpu_6502_reset(&cpu);
        }
        else if (command.equals("SLOW"))
        {
            cpu_6502_set_clock(&cpu, 40);
            log("Clock set to 40Hz");
        }
        else if (command.equals("MEDIUM"))
        {
            cpu_6502_set_clock(&cpu, 500);
            log("Clock set to 500Hz");
        }
        else if (command.equals("FAST"))
        {
            cpu_6502_set_clock(&cpu, 20000);
            log("Clock set to 20kHz");
        }
        else if (command.equals("TURBO"))
        {
            cpu_6502_set_clock(&cpu, 100000);
            log("Clock set to 100kHz");
        }
        else if (command.equals("MAX"))
        {
            cpu_6502_set_clock(&cpu, 500000);
            log("Clock set to 500kHz");
        }
        else if (command.equals("ls"))
        {
            list_files();
        }
        else if (command.substring(0, 7).equals("hexdump"))
        {
            char filename[255];
            command.substring(8, 255 + 8).toCharArray(filename, 255);

            Serial.print("Hexdump: ");
            Serial.println(filename);

            hexdump(filename);
        }
        else if (command.substring(0, 2).equals("rd"))
        {
            char addr[5];
            command.substring(3, 7).toCharArray(addr, 5);
            uint16_t A = (uint16_t)strtol(addr, NULL, 16);

            Serial.print(A, HEX);
            Serial.print(": ");
            Serial.print(read_ram(A), HEX);
            Serial.println();
        }
        else if (command.substring(0, 2).equals("wr"))
        {
            char addr[5];
            char val[3];
            command.substring(3, 7).toCharArray(addr, 5);
            uint16_t A = (uint16_t)strtol(addr, NULL, 16);

            command.substring(8, 10).toCharArray(val, 3);
            uint8_t D = (uint8_t)strtol(val, NULL, 16);

            Serial.print("write: ");
            Serial.print(D, HEX);
            Serial.print(" to ");
            Serial.print(A, HEX);
            Serial.println();

            write_ram(A, D);
        }
        else if (command.equals("buson"))
        {
            Serial.println("BE on");

            // Set data bus to input
            DDR_DATA = 0x00;

            //Set address bus to input
            DDR_ADDR_HIGH = 0x00;
            DDR_ADDR_LOW = 0x00;

            digitalWrite(BE_OUT, HIGH);
        }
        else if (command.equals("busoff"))
        {
            Serial.println("BE off");
            digitalWrite(BE_OUT, LOW);
        }
        else if (!cpu.Running)
        {
            cpu_6502_cycle(&cpu, 1);

            // Print the buses after each cycle when single-stepping
            cpu_6502_dump(&cpu, log_message);
            log(log_message);
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
            log(log_message);
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
