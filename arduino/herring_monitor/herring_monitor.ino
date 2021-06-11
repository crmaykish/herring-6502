#include "herring_pins.h"

//#include <LiquidCrystal.h>

#define CLOCK_DELAY 200

//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool running = true;
bool clock_state = false;

//char lcd_line_1[16];
//char lcd_line_2[16];

void cycle(int count, int wait)
{
  for (int i = 0; i < count; i++)
  {
    digitalWrite(CLK, LOW);
    delay(wait);
    digitalWrite(CLK, HIGH);
    delay(wait);
  }
}

void reset()
{
  Serial.println("Resetting");
  digitalWrite(RST, LOW);
  delay(100);

  cycle(1, 50);

  digitalWrite(RST, HIGH);
  delay(100);
}

void printState()
{
  bool a0 = digitalRead(A0);
  bool a1 = digitalRead(A1);
  bool a2 = digitalRead(A2);
  bool a3 = digitalRead(A3);
  bool a4 = digitalRead(A4);
  bool a5 = digitalRead(A5);
  bool a6 = digitalRead(A6);
  bool a7 = digitalRead(A7);
  bool a8 = digitalRead(A8);
  bool a9 = digitalRead(A9);
  bool a10 = digitalRead(A10);
  bool a11 = digitalRead(A11);
  bool a12 = digitalRead(A12);
  bool a13 = digitalRead(A13);
  bool a14 = digitalRead(A14);
  bool a15 = digitalRead(A15);

  uint16_t A = a0;
  A += (a1 << 1);
  A += (a2 << 2);
  A += (a3 << 3);
  A += (a4 << 4);
  A += (a5 << 5);
  A += (a6 << 6);
  A += (a7 << 7);
  A += (a8 << 8);
  A += (a9 << 9);
  A += (a10 << 10);
  A += (a11 << 11);
  A += (a12 << 12);
  A += (a13 << 13);
  A += (a14 << 14);
  A += (a15 << 15);

  bool d0 = digitalRead(D0);
  bool d1 = digitalRead(D1);
  bool d2 = digitalRead(D2);
  bool d3 = digitalRead(D3);
  bool d4 = digitalRead(D4);
  bool d5 = digitalRead(D5);
  bool d6 = digitalRead(D6);
  bool d7 = digitalRead(D7);

  uint8_t D = d0;
  D += d1 << 1;
  D += d2 << 2;
  D += d3 << 3;
  D += d4 << 4;
  D += d5 << 5;
  D += d6 << 6;
  D += d7 << 7;

  char rw = 'R';

  if (digitalRead(RW) == LOW)
  {
    rw = 'W';
  }

//  sprintf(lcd_line_1, "ADDR: %04X", A);
//  sprintf(lcd_line_2, "DATA: %02X      %c", D, rw);
  
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print(lcd_line_1);
//  lcd.setCursor(0,1);
//  lcd.print(lcd_line_2);

  Serial.print("ADDR: ");
  Serial.print(A, HEX);

  Serial.print(" | DATA: ");
  Serial.print(D, HEX);

  if (digitalRead(RW) == HIGH)
  {
    Serial.print(" | R");
  }
  else
  {
    Serial.print(" | W");
  }

  Serial.println();
}

void setup()
{
  // Control Pins
  pinMode(CLK, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(RW, INPUT);

  // Data Pins
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);

  // Address Pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);

  digitalWrite(RST, HIGH);
  digitalWrite(CLK, HIGH);

//  lcd.begin(16, 2);

//  lcd.clear();

  // Start serial monitor
  Serial.begin(115200);

  reset();

  running = true;
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');

    if (command.equals("RUN"))
    {
      Serial.println("Run");
      running = true;
    }
    else if (command.equals("STOP"))
    {
      Serial.println("Stop");
      running = false;
      clock_state = LOW;
      digitalWrite(CLK, LOW);
    }
    else if (command.equals("RESET"))
    {
      reset();
    }
    else if (!running)
    {
      Serial.println("Cycling");
      cycle(1, CLOCK_DELAY);
      printState();
    }
  }

  if (running)
  {
    if (clock_state == LOW)
    {
      clock_state = HIGH;
    }
    else
    {
      clock_state = LOW;
    }

    digitalWrite(CLK, clock_state);

    delay(CLOCK_DELAY);

    printState();
  }
}
