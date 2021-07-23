#include <PS2KeyAdvanced.h>
#include <PS2KeyMap.h>

#define DATA_PIN 4
#define CLK_PIN 3

#define BAUDRATE 19200

PS2KeyAdvanced keyboard;
PS2KeyMap keymap;

void setup()
{
    Serial.begin(19200);

    keyboard.begin(DATA_PIN, CLK_PIN);
    keyboard.setNoBreak(1);
    keyboard.setNoRepeat(1);

    keymap.selectMap((char *)"US");
}

void loop()
{
    uint16_t code = keyboard.available();

    if (code > 0)
    {
        code = keyboard.read();
        code = (keymap.remapKey(code) & 0xFF);

        if (code > 0)
        {
            Serial.write(code & 0xFF);
        }
    }
}
