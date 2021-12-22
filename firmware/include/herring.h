#ifndef HERRING_H
#define HERRING_H

// Start of program RAM
#define PROGRAM_RAM 0x1000

// VIA registers
#define VIA0_PORTB 0x8100
#define VIA0_PORTA 0x8101
#define VIA0_DDRB 0x8102
#define VIA0_DDRA 0x8103

// VIA DDR control
#define VIA_OUTPUT 0xFF
#define VIA_INPUT 0x00

// ACIA 0 registers
#define ACIA0_DATA 0x8000
#define ACIA0_STATUS 0x8001
#define ACIA0_COMMAND 0x8002
#define ACIA0_CONTROL 0x8003

// ACIA 1 registers
#define ACIA1_DATA 0x8200
#define ACIA1_STATUS 0x8201
#define ACIA1_COMMAND 0x8202
#define ACIA1_CONTROL 0x8203

// ACIA flags
#define ACIA_READY_RX 0x08
#define ACIA_READY_TX 0x10

// Send a dummy value to the ACIA STATUS register to reset it
#define ACIA_RESET 0x00

// RTS low, transmit interrupt disabled, receiver interrupt disabled, DTR low
#define ACIA_COMMAND_INIT 0x0B

// Baudrate 1/16 oscillator, 8 data bits, 1 stop bit
#define ACIA_CONTROL_BAUD 0b00010000

// Useful ASCII characters
#define ASCII_NEWLINE 0x0A
#define ASCII_RETURN 0x0D
#define ASCII_ZERO 0x00
#define ASCII_ESC 0x1B

#endif
