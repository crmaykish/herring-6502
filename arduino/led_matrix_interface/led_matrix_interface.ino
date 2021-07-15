#include <RGBmatrixPanel.h>

#define CLK 11
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3

// Data bus
const int command_bus[] = {14, 15, 16, 17, 18, 19, 20, 21}; //port A

// Address pins
const int data_bus[] = {31, 33, 35, 37, 39, 41, 43, 45}; // port B

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

int command = 0;
int data = 0;

void setup()
{
    // Set address bus to outputs
    for (int i = 0; i < 8; i++)
    {
        pinMode(command_bus[i], INPUT);
        pinMode(data_bus[i], INPUT);
    }

    Serial.begin(115200);

    matrix.begin();
    matrix.fillScreen(matrix.Color333(0, 0, 0));

    attachInterrupt(digitalPinToInterrupt(20), command_handler, RISING);
}

void loop()
{
//    Serial.print("Command: ");
//    Serial.print(command, BIN);
//    Serial.print(" | Data: ");
//    Serial.print(data, BIN);
//    Serial.println();
//
//    delay(500);
}

int readBus(const int *bus)
{
    int total = 0;

    for (int i = 0; i < 8; i++)
    {
        total |= (digitalRead(bus[i]) << i);
    }

    return total;
}

void command_handler()
{
    command = readBus(command_bus);
    data = readBus(data_bus);

    if (command & 0b10000000)
    {
      int x = command & 0b00111111;
      int y = data & 0b00011111;

      matrix.drawPixel(x, y, matrix.Color333(0, 7, 1)); 
    }
    else
    {
      matrix.fillScreen(matrix.Color333(0, 0, 0));
    }
}
