#define RESET 13
#define CLOCK 11

int resetTime = 500;
long clock_frequency = 1000;

void setup() {
  // put your setup code here, to run once:
  pinMode(RESET, OUTPUT);
  pinMode(CLOCK, OUTPUT);

  digitalWrite(CLOCK, HIGH);
  digitalWrite(RESET, LOW);
  delay(resetTime);
  digitalWrite(RESET, HIGH);

  if (clock_frequency >= 50){
    tone(CLOCK, clock_frequency);
  }
}

void loop() {
  if (clock_frequency < 50)
  {
    digitalWrite(CLOCK, LOW);
    delay(1000 / clock_frequency);
    digitalWrite(CLOCK, HIGH);
    delay(1000 / clock_frequency);
  }
}
