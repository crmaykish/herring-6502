#include <SPI.h>
#include <SD.h>

#define DATABUS VPORTD

const int chipSelect = 9;

File textFile;

void setup() {

  VPORTD.DIR |= 0xFF;
  
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Setting up SD card");

  if (!SD.begin(chipSelect))
  {
    Serial.println("No SD card");
    while(1);
  }

  Serial.println("SD Ready");

  textFile = SD.open("test.txt");

  if (textFile)
  {
    while (textFile.available())
    {
      byte b = textFile.read();

      Serial.write(b);
      VPORTD.OUT = b;

      delay(200);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
