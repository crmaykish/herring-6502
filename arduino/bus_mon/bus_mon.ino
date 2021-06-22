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

// Pin Definitions
#define PORT_ADDR_HIGH PORTA
#define PORT_ADDR_LOW PORTC
#define PORT_DATA PORTF
#define PORT_CTRL PORTK

#define PIN_ADDR_HIGH PINA
#define PIN_ADDR_LOW PINC
#define PIN_DATA PINF
#define PIN_CTRL PINK

#define DDR_ADDR_HIGH DDRA
#define DDR_ADDR_LOW DDRC
#define DDR_DATA DDRF
#define DDR_CTRL DDRK

#define RST_OUT 13
#define CLK_OUT 11

// Control flag indices
#define RESB_IN 7
#define RDY_IN 6
#define IRQB_IN 5
#define PHI2_IN 4
#define MLB_IN 3
#define NMIB_IN 2
#define SYNC_IN 1
#define RWB_IN 0

// WARNING: At some point, the serial monitor will no longer be able to keep up with the clock speed
// and will start missing changes on the buses

#define DEFAULT_CLOCK_SPEED_HZ 40

#define SERIAL_BAUDRATE 115200

typedef struct
{
    bool Running;
    uint16_t AddressBus;
    uint16_t PrevAddressBus;
    uint8_t DataBus;
    uint8_t PrevDataBus;
    uint8_t PrevControlFlags;
    uint8_t ControlFlags;
    unsigned int ClockSpeed;
} CPU_6502_t;

static CPU_6502_t cpu = {
    .Running = true,
    .AddressBus = 0x0000,
    .PrevAddressBus = 0x0000,
    .DataBus = 0x00,
    .PrevDataBus = 0x00,
    .PrevControlFlags = 0x00,
    .ControlFlags = 0x00,
    .ClockSpeed = DEFAULT_CLOCK_SPEED_HZ};

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
    sprintf(log_message, "%04X | %02X | %d%d%d%d%d%d%d%d",
            cpu.AddressBus,
            cpu.DataBus,
            (cpu.ControlFlags & 0b10000000) >> 7,
            (cpu.ControlFlags & 0b1000000) >> 6,
            (cpu.ControlFlags & 0b100000) >> 5,
            (cpu.ControlFlags & 0b10000) >> 4,
            (cpu.ControlFlags & 0b1000) >> 3,
            (cpu.ControlFlags & 0b100) >> 2,
            (cpu.ControlFlags & 0b10) >> 1,
            cpu.ControlFlags & 0b1);
    log(log_message);
}

void cpu_cycle(int count)
{
    for (int i = 0; i < count; i++)
    {
        digitalWrite(CLK_OUT, LOW);
        delay(10);
        digitalWrite(CLK_OUT, HIGH);
        delay(10);
    }

    cpu.PrevAddressBus = cpu.AddressBus;
    cpu.PrevDataBus = cpu.DataBus;
    cpu.PrevControlFlags = cpu.ControlFlags;

    // Read the CPU buses
    cpu.AddressBus = (reverse_bits(PIN_ADDR_HIGH) << 8) + PIN_ADDR_LOW;
    cpu.DataBus = PIN_DATA;
    cpu.ControlFlags = PIN_CTRL;
}

void cpu_reset()
{
    log("Resetting 65C02...");
    digitalWrite(RST_OUT, LOW);
    delay(100);
    cpu_cycle(1);
    digitalWrite(RST_OUT, HIGH);
    delay(100);
}

void setup()
{
    // Control Pins
    pinMode(CLK_OUT, OUTPUT);
    pinMode(RST_OUT, OUTPUT);

    // Set address, data, and control buses to inputs
    DDR_ADDR_HIGH |= 0x00;
    DDR_ADDR_LOW |= 0x00;
    DDR_DATA |= 0x00;
    DDR_CTRL |= 0x00;

    pinMode(RST_OUT, OUTPUT);
    pinMode(CLK_OUT, OUTPUT);

    // Start serial monitor
    Serial.begin(SERIAL_BAUDRATE);

    tone(CLK_OUT, cpu.ClockSpeed);
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
            tone(CLK_OUT, cpu.ClockSpeed);
        }
        else if (command.equals("STOP"))
        {
            log("Stop");
            cpu.Running = false;
            noTone(CLK_OUT);
            digitalWrite(CLK_OUT, HIGH);
        }
        else if (command.equals("RESET"))
        {
            cpu_reset();
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
            cpu_cycle(1);

            // Print the buses after each cycle when single-stepping
            dump_cpu_state();
        }
    }

    if (cpu.Running)
    {
        cpu_cycle(0);

        if (cpu.AddressBus != cpu.PrevAddressBus ||
            cpu.DataBus != cpu.PrevDataBus ||
            cpu.ControlFlags != cpu.PrevControlFlags)
        {
            // Only print the buses if something has changed
            dump_cpu_state();
        }
    }
}
