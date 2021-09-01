#define SERIAL_BAUDRATE 115200
#define CLOCK_SPEED_HZ 1000

// Clock
#define CLK_IN PIN_PC2
#define CLK_OUT PIN_PC3

// I/O Pins
#define IO_0 PIN_PC4
#define IO_1 PIN_PC4

// Control pins
#define BUS_EN PIN_PE1
#define RWB PIN_PE2
#define RESB PIN_PE3

// Data bus
#define PORT_DATA VPORTD

// Address bus
#define A15 PIN_PE0
#define PORT_ADDR_HIGH VPORTF
#define PORT_ADDR_LOW VPORTA

uint8_t reverse_bits(uint8_t b);

uint8_t volatile data_bus = 0;
uint16_t volatile addr_bus = 0;
bool volatile rw = 0;

void write_ram(uint16_t addr, uint8_t val)
{
  PORT_ADDR_LOW.OUT = reverse_bits(addr & 0xFF);
  PORT_ADDR_HIGH.OUT = reverse_bits((addr & 0x7F) >> 8);
  digitalWrite(A15, (addr & 0x80) >> 8);

  PORT_DATA.OUT = val;

  // toggle the write enable pin
  digitalWrite(RWB, LOW);
  delay(5);
  digitalWrite(RWB, HIGH);
  delay(5);
}

void setup() {
  Serial1.begin(SERIAL_BAUDRATE);

  pinMode(CLK_IN, OUTPUT);
  pinMode(CLK_OUT, INPUT);
  pinMode(BUS_EN, OUTPUT);
  pinMode(RESB, OUTPUT);
  pinMode(RWB, INPUT);

  // Set data bus to input
  PORT_DATA.DIR = 0;

  // Set address bus to input
  pinMode(A15, INPUT);
  PORT_ADDR_HIGH.DIR = 0;
  PORT_ADDR_LOW.DIR = 0;

  Serial1.println("Bus Enable HIGH");

  // Enable the bus
  digitalWrite(BUS_EN, HIGH);

  Serial1.println("Reset CPU");

  // Reset CPU
  digitalWrite(RESB, HIGH);
  delay(10);
  digitalWrite(RESB, LOW);
  delay(100);
  digitalWrite(RESB, HIGH);
  delay(10);

  attachInterrupt(digitalPinToInterrupt(CLK_OUT), clock_interrupt, RISING);

  delay(20);

  tone(CLK_IN, CLOCK_SPEED_HZ);
}

void loop() {

}

void clock_interrupt()
{
  addr_bus = (digitalRead(A15) << 15) + (reverse_bits(PORT_ADDR_HIGH.IN) << 7) + reverse_bits(PORT_ADDR_LOW.IN);
  data_bus = PORT_DATA.IN;
  rw = digitalRead(RWB);

  Serial1.print(rw ? "R | " : "W | ");
  Serial1.print(addr_bus, HEX);
  Serial1.print(": ");
  Serial1.print(data_bus, HEX);
  Serial1.println();
}

uint8_t reverse_bits(uint8_t b)
{
    return ((b & 0b1) << 7) |
           ((b & 0b10) << 5) |
           ((b & 0b100) << 3) |
           ((b & 0b1000) << 1) |
           ((b & 0b10000) >> 1) |
           ((b & 0b100000) >> 3) |
           ((b & 0b1000000) >> 5) |
           ((b & 0b10000000) >> 7);
}
