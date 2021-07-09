#ifndef BUS_MON_H
#define BUS_MON_H

// Ports
#define PORT_ADDR_HIGH PORTA
#define PORT_ADDR_LOW PORTC
#define PORT_DATA PORTF
#define PORT_CTRL PORTK

// Pins
#define PIN_ADDR_HIGH PINA
#define PIN_ADDR_LOW PINC
#define PIN_DATA PINF
#define PIN_CTRL PINK

// DDR Registers
#define DDR_ADDR_HIGH DDRA
#define DDR_ADDR_LOW DDRC
#define DDR_DATA DDRF
#define DDR_CTRL DDRK

// Outputs
#define RST_OUT 13
#define CLK_OUT 11

#define SD_SELECT 49

// Control flag indices
#define RESB_IN 7
#define RDY_IN 6
#define IRQB_IN 5
#define PHI2_IN 4
#define MLB_IN 3
#define NMIB_IN 2
#define SYNC_IN 1
#define RWB_IN 0

#define DEFAULT_CLOCK_SPEED_HZ 40

#define SERIAL_BAUDRATE 115200

void log(String s);

uint8_t reverse_bits(uint8_t b);

#endif
