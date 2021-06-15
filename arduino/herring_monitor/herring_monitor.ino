/**
 * @file herring_monitor.ino
 * @author Colin Maykish
 * @brief Serial Monitor for the Herring 6502 computer system
 * Reads the data and address buses and controls the clock and reset pins for the 6502
 * 
 * @date 2021-06-11
 * 
 * @copyright Copyright (c) 2021
 */

// Pin Definitions
#define PORT_ADDR_HIGH PORTB
#define PORT_ADDR_LOW PORTA
#define PORT_DATA PORTC

#define PIN_ADDR_HIGH PINB
#define PIN_ADDR_LOW PINA
#define PIN_DATA PINC

#define DDR_ADDR_HIGH DDRB
#define DDR_ADDR_LOW DDRA
#define DDR_DATA DDRC

#define RST 12
#define CLK 13
#define RW 14

#define CLOCK_DELAY 10

typedef struct
{
    bool Running;
    bool ReadWrite;
    uint16_t AddressBus;
    uint8_t DataBus;
} CPU_6502_t;

static CPU_6502_t cpu = {
    .Running = false,
    .ReadWrite = false,
    .AddressBus = 0x0000,
    .DataBus = 0x00};

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

static char log_message[64];

void log(String s)
{
    Serial.println(s);
}

void dump_cpu_state()
{
    sprintf(log_message, "ADDR: %04X | DATA: %02X | RW: %d", cpu.AddressBus, cpu.DataBus, cpu.ReadWrite);
    log(log_message);
}

void cpu_cycle(int count)
{
    for (int i = 0; i < count; i++)
    {
        digitalWrite(CLK, LOW);
        delay(CLOCK_DELAY);
        digitalWrite(CLK, HIGH);
        delay(CLOCK_DELAY);
    }

    // Read the CPU buses
    cpu.AddressBus = (reverse_bits(PIN_ADDR_HIGH) << 8) + reverse_bits(PIN_ADDR_LOW);
    cpu.DataBus = reverse_bits(PIN_DATA);
    cpu.ReadWrite = digitalRead(RW);
}

void cpu_reset()
{
    cpu_cycle(1);
    log("Resetting 65C02...");
    digitalWrite(RST, LOW);
    cpu_cycle(1);
    digitalWrite(RST, HIGH);
}

void setup()
{
    // Control Pins
    pinMode(CLK, OUTPUT);
    pinMode(RST, OUTPUT);
    pinMode(RW, INPUT);

    // Set address and data buses as inputs
    DDR_ADDR_HIGH |= 0x00;
    DDR_ADDR_LOW |= 0x00;
    DDR_DATA |= 0x00;

    pinMode(RST, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(RW, INPUT);

    // Start serial monitor
    Serial.begin(115200);

    cpu_reset();
}

void loop()
{
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');

        if (command.equals("RUN"))
        {
            log("Run");
            cpu.Running = true;
        }
        else if (command.equals("STOP"))
        {
            log("Stop");
            cpu.Running = false;
            digitalWrite(CLK, LOW);
        }
        else if (command.equals("RESET"))
        {
            cpu_reset();
        }
        else if (!cpu.Running)
        {
            cpu_cycle(1);
            dump_cpu_state();
        }
    }

    if (cpu.Running)
    {
        cpu_cycle(1);
        dump_cpu_state();
    }
}
