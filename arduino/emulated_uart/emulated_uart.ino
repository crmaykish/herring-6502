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

byte buffer_out[256];
uint8_t buffer_out_index = 0;
bool out_ready = false;
bool out_ack = false;

bool is_ascii(byte b)
{
    return (b == 10) || (b >= 32 && b <= 127);
}

void data_bus_mode(uint8_t mode)
{
    pinMode(D0, mode);
    pinMode(D1, mode);
    pinMode(D2, mode);
    pinMode(D3, mode);
    pinMode(D4, mode);
    pinMode(D5, mode);
    pinMode(D6, mode);
    pinMode(D7, mode);
}

void byte_to_bus(byte b)
{
    digitalWrite(D0, b & 0x1);
    digitalWrite(D1, (b & 0x10) >> 1);
    digitalWrite(D1, (b & 0x100) >> 2);
    digitalWrite(D1, (b & 0x1000) >> 3);
    digitalWrite(D1, (b & 0x10000) >> 4);
    digitalWrite(D1, (b & 0x100000) >> 5);
    digitalWrite(D1, (b & 0x1000000) >> 6);
    digitalWrite(D1, (b & 0x10000000) >> 7);
}

void setup()
{
    data_bus_mode(INPUT);

    pinMode(ADDR0, INPUT);
    pinMode(ADDR1, INPUT);
    pinMode(ADDR2, INPUT);
    pinMode(ADDR3, INPUT);

    pinMode(Y, INPUT);
    pinMode(Z, INPUT);
    pinMode(RW, INPUT);
    pinMode(CLK, INPUT);

    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("Starting");
}

void loop()
{
    if (Serial.available() > 1)
    {
        if (buffer_out_index < sizeof(buffer_out))
        {
            buffer_out[buffer_out_index] = Serial.read();
            buffer_out_index++;
        }
        else
        {
            Serial.println("DEBUG: Buffer is full.");
        }
    }

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
                data_bus_mode(INPUT);

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
                if (buffer_out_index == 0)
                {
                    
                }
                Serial.write(buffer_out);

                buffer_in = 0;
                in_ready = false;
                submit_low = false;
                submit_high = false;
            }
        }
        else
        {
            if (AddressBus == 0x10)
            {
                // CPU is requesting a byte
                data_bus_mode(OUTPUT);
                byte_to_bus(0xAB);
            }

            if (AddressBus == 0x11)
            {
                // CPU acks the byte
            }
        }
    }
}
