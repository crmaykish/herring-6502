#define SERIAL_BAUDRATE 115200
#define CLOCK_DELAY 250

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

  pinMode(CLK_IN, INPUT);
  pinMode(CLK_OUT, INPUT);
  pinMode(BUS_EN, OUTPUT);
  pinMode(RESB, OUTPUT);

  // Load program code to RAM

  Serial1.println("Load program into RAM...");

  digitalWrite(BUS_EN, LOW);

  pinMode(A15, OUTPUT);
  PORT_ADDR_HIGH.DIR = 0xFF;
  PORT_ADDR_LOW.DIR = 0xFF;  

  PORT_DATA.DIR = 0xFF;

  pinMode(RWB, OUTPUT);
  digitalWrite(RWB, HIGH);

  uint8_t program[] = {0xa9, 0x00, 0x8d, 0x01, 0xf8, 0xa9, 0x0b, 0x8d, 0x02, 0xf8, 0xa9, 0x1f, 0x8d, 0x03, 0xf8, 0xa9, 0x00, 0x8d, 0x02, 0xf4, 0x8d, 0x03, 0xf4, 0x8d, 0x00, 0xf4, 0x8d, 0x01, 0xf4, 0xa2, 0x20, 0x8e, 0x00, 0xf8, 0x8e, 0x00, 0xf4, 0x8e, 0x01, 0xf4, 0xe0, 0x7e, 0xf0, 0xf1, 0xe8, 0x4c, 0x1f, 0x02};

  for (int i = 0; i < sizeof(program); i++)
  {
    uint16_t addr = 0x200 + i;
    
    write_ram(addr, program[i]);
  }

  // Write the reset vector (0x200)
  write_ram(0xFFFC, 0x00);
  write_ram(0xFFFD, 0x02);

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
}

void loop() {

}

void clock_interrupt()
{
  addr_bus = (digitalRead(A15) << 15) + (reverse_bits(PORT_ADDR_HIGH.IN) << 7) + reverse_bits(PORT_ADDR_LOW.IN);
  data_bus = PORT_DATA.IN;

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
