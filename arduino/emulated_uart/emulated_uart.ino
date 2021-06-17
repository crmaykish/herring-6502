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

byte AddressBus = 0x0;
byte DataBus = 0x0;

byte buffer_in = 0x0;
bool in_ready = false;

bool submit_low = false;
bool submit_high = false;

bool is_ascii(byte b)
{
    return (b == 10) || (b >= 32 && b <= 127);
}

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
            // 1. Watch for a data value

            if (AddressBus == 0x00)
            {
                if (is_ascii(DataBus)) // TODO: This is hacky, binary data should be acceptable, but we're getting garbage data from somewhere
                {
                    buffer_in = DataBus;
                    in_ready = true;
                }
            }

            // 2. Watch for the command to go low and then high
            if (in_ready)
            {
                if (AddressBus == 0x01 && DataBus == 0x00)
                {
                    submit_low = true;
                }
                if (AddressBus == 0x01 && DataBus == 0x01)
                {
                    submit_high = true;
                }
            }

            // 3. Write the byte to the serial port
            if (in_ready && submit_low && submit_high)
            {
                Serial.write(buffer_in);

                buffer_in = 0;
                in_ready = false;
                submit_low = false;
                submit_high = false;
            }
        }
        else
        {
            // TODO: CPU is reading from UART
        }
    }
}
