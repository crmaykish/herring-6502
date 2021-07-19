#define STEP PB4
#define RUN_STOP PB3
#define CLK PB2 // Clock output to the 6502 bus (PHI0)
#define RST PB1 // Reset output to the 6502 bus

// TODO: start with the clock disabled, hold RUN/STOP for 500ms to activate clock
// TODO: manual clock control should be disabled when an external oscillator is running
// TODO: make sure the reset button behaves as expected

#define CLOCK_DELAY 250
#define DEBOUNCE_LIMIT 50

unsigned long lastTime = 0;
byte clock_state = HIGH;

unsigned long lastBounce = 0;

byte clock_running = LOW;

void setup() {
  pinMode(STEP, INPUT);
  pinMode(RUN_STOP, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(RST, OUTPUT);

  // Clock always starts high
  digitalWrite(CLK, clock_state);

  // TODO: I don't think this will do anything unless the clock is running
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(10);
}

void loop() {
  // Input checking
  if (digitalRead(RUN_STOP) == LOW)
  {
    delay(DEBOUNCE_LIMIT);

    if (clock_running == LOW)
    {
      clock_running = HIGH;
    }
    else
    {
      clock_running = LOW;
      clock_state = HIGH;
    }
  }

  // Clock pulsing
  if (clock_running == HIGH){
    unsigned long currentTime = millis();
  
    if (currentTime - lastTime >= CLOCK_DELAY)
    {
      lastTime = currentTime;
      
      if (clock_state == LOW)
      {
        clock_state = HIGH;
      }
      else {
        clock_state = LOW;
      }
    }
  
    digitalWrite(CLK, clock_state);
  }
}
