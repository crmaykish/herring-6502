#include <RGBmatrixPanel.h>

#define CLK 11
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

bool textMode = false;

void setup()
{
    matrix.begin();
    matrix.fillScreen(matrix.Color333(0, 0, 0));
    Serial.begin(19200);
    matrix.setCursor(0, 0);
    matrix.setTextColor(matrix.Color333(0, 7, 1));
}

void loop()
{
    if (textMode)
    {
        if (Serial.available() > 0)
        {
            byte b = Serial.read();

            // new line
            if (b == 0x0A)
            {
                matrix.setCursor(matrix.getCursorX(), matrix.getCursorY() + 8);
            }
            // carriage return
            else if (b == 0x0D)
            {
                matrix.setCursor(0, matrix.getCursorY() + 8);
            }
            else if (b == 0x03)
            {
                matrix.fillScreen(matrix.Color333(0, 0, 0));
                matrix.setCursor(0, 0);
            }
            else
            {
                matrix.write(b);
            }

            Serial.write(b);
        }
    }

    else
    {
        // TODO: Crude serial-to-pixel conversion
    }
}