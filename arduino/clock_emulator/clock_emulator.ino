/**
 * @file clock_emulator.ino
 * @author Colin Maykish (crmaykish@gmail.com)
 * @brief Basic clock and single-step controller for the Herring 6502 computer. Designed for an ATtiny85.
 * @date 2021-07-28
 * 
 * @copyright Copyright (c) 2021
 */

#define RESET PIN_PB1
#define CLOCK PIN_PB2
#define RUN_STOP PIN_PB3
#define STEP PIN_PB4

const int resetTime = 500;
const long clock_frequency = 5;
const unsigned long debounceDelay = 50;
const int singleStepDelay = 100;

bool running = false;

unsigned long lastRunStopTime = 0;
unsigned long lastStepTime = 0;

int runStopState;
int lastrunStopState = HIGH;

int stepState;
int lastStepState = HIGH;

unsigned long previousTime = 0;

int clockState = HIGH;

void setup()
{
  pinMode(RESET, OUTPUT);

  // Clock needs to start at a high-impedance state, assuming it will be driven externally
  pinMode(CLOCK, INPUT);

  digitalWrite(RESET, LOW);
  delay(resetTime);
  digitalWrite(RESET, HIGH);
}

void loop()
{
  // Cycle the clock
  if (running)
  {
    unsigned long currentTime = millis();

    if ((currentTime - previousTime) >= ((1000 / clock_frequency) / 2))
    {
      previousTime = currentTime;

      if (clockState == LOW)
      {
        clockState = HIGH;
      }
      else
      {
        clockState = LOW;
      }

      digitalWrite(CLOCK, clockState);
    }
  }
  else
  {
    // Watch for STEP button presses
    int reading = digitalRead(STEP);

    if (reading != lastStepState)
    {
      lastStepTime = millis();
    }

    if ((millis() - lastStepTime) > debounceDelay)
    {
      if (reading != stepState)
      {
        stepState = reading;

        if (stepState == LOW)
        {
          // Single-step the clock
          digitalWrite(CLOCK, LOW);
          delay(singleStepDelay);
          digitalWrite(CLOCK, HIGH);
          delay(singleStepDelay);
        }
      }
    }

    lastStepState = reading;
  }

  // Watch for RUN/STOP button presses
  int reading = digitalRead(RUN_STOP);

  if (reading != lastrunStopState)
  {
    lastRunStopTime = millis();
  }

  if ((millis() - lastRunStopTime) > debounceDelay)
  {
    if (reading != runStopState)
    {
      runStopState = reading;

      if (runStopState == LOW)
      {
        running = !running;

        if (running)
        {
          pinMode(CLOCK, OUTPUT);
        }
        else
        {
          pinMode(CLOCK, INPUT);
        }
      }
    }
  }

  lastrunStopState = reading;
}
