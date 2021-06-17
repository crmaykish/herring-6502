#define SERIAL_BAUDRATE 9600

#define ADDR0 13
#define ADDR1 12
#define ADDR2 11
#define ADDR3 10

#define D0 2
#define D1 3
#define D2 4
#define D3 5
#define D4 6
#define D5 7
#define D6 8
#define D7 9

#define Y A0
#define Z A1
#define RW A2
#define CLK A3

static byte AddressBus = 0x0;
static byte DataBus = 0x0;

static byte buffer_in = 0x0;
static byte command_in = 0x0;
static bool in_ready = false;

void setup()
{
    pinMode(ADDR0, INPUT);
    pinMode(ADDR1, INPUT);
    pinMode(ADDR2, INPUT);
    pinMode(ADDR3, INPUT);

    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);

    pinMode(Y, INPUT);
    pinMode(Z, INPUT);
    pinMode(RW, INPUT);
    pinMode(CLK, INPUT);

    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("Starting");
}

void loop()
{
    if (!digitalRead(Y) && digitalRead(Z))
    {
        // Chip is enabled

        // Read buses
        DataBus = digitalRead(D0);
        DataBus += (digitalRead(D1) << 1);
        DataBus += (digitalRead(D2) << 2);
        DataBus += (digitalRead(D3) << 3);
        DataBus += (digitalRead(D4) << 4);
        DataBus += (digitalRead(D5) << 5);
        DataBus += (digitalRead(D6) << 6);
        DataBus += (digitalRead(D7) << 7);

        AddressBus = digitalRead(ADDR0);
        AddressBus += (digitalRead(ADDR1) << 1);
        AddressBus += (digitalRead(ADDR2) << 2);
        AddressBus += (digitalRead(ADDR3) << 3);

        if (!digitalRead(RW))
        {
            // CPU is writing to UART

            // Command/data decoding
            switch (AddressBus)
            {
            case 0x00:
                buffer_in = DataBus;
                in_ready = true;
                break;
            case 0x01:
                command_in = DataBus;
            default:
                break;
            }

            if (in_ready && command_in == 0b01010101)
            {
                // Write the charater
                // TODO: This is a hack to drop non-ASCII characters, but why are we still getting them at all?
                if ((buffer_in >= 32 && buffer_in <= 126) || buffer_in == 10)
                {
                    Serial.write(buffer_in);
                }

                // Clear the buffers
                buffer_in = 0x00;
                command_in = 0x00;
                in_ready = false;
            }
        }
        else
        {
            // TODO: CPU is reading from UART
        }
    }
}
