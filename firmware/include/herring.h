#ifndef HERRING_H
#define HERRING_H

#include <stdint.h>

// Start of program RAM
#define PROGRAM_RAM 0x1000

#define VIA0 0xC100
#define VIA1 0xC180
#define VIA2 0xC200

// VIA DDR control
#define VIA_OUTPUT 0xFF
#define VIA_INPUT 0x00

// VIA0 registers
#define VIA0_PORTB VIA0 + 0x0
#define VIA0_PORTA VIA0 + 0x1
#define VIA0_DDRB VIA0 + 0x2
#define VIA0_DDRA VIA0 + 0x3
#define VIA0_T1CL VIA0 + 0x4
#define VIA0_T1CH VIA0 + 0x5
#define VIA0_T1LL VIA0 + 0x6
#define VIA0_T1LH VIA0 + 0x7
#define VIA0_T2CL VIA0 + 0x8
#define VIA0_T2CH VIA0 + 0x9
#define VIA0_SR VIA0 + 0xA
#define VIA0_ACR VIA0 + 0xB
#define VIA0_PCR VIA0 + 0xC
#define VIA0_IFR VIA0 + 0xD
#define VIA0_IER VIA0 + 0xE

// VIA1 registers
#define VIA1_PORTB VIA1 + 0x0
#define VIA1_PORTA VIA1 + 0x1
#define VIA1_DDRB VIA1 + 0x2
#define VIA1_DDRA VIA1 + 0x3
#define VIA1_T1CL VIA1 + 0x4
#define VIA1_T1CH VIA1 + 0x5
#define VIA1_T1LL VIA1 + 0x6
#define VIA1_T1LH VIA1 + 0x7
#define VIA1_T2CL VIA1 + 0x8
#define VIA1_T2CH VIA1 + 0x9
#define VIA1_SR VIA1 + 0xA
#define VIA1_ACR VIA1 + 0xB
#define VIA1_PCR VIA1 + 0xC
#define VIA1_IFR VIA1 + 0xD
#define VIA1_IER VIA1 + 0xE

// VIA2 registers
#define VIA2_PORTB VIA2 + 0x0
#define VIA2_PORTA VIA2 + 0x1
#define VIA2_DDRB VIA2 + 0x2
#define VIA2_DDRA VIA2 + 0x3
#define VIA2_T1CL VIA2 + 0x4
#define VIA2_T1CH VIA2 + 0x5
#define VIA2_T1LL VIA2 + 0x6
#define VIA2_T1LH VIA2 + 0x7
#define VIA2_T2CL VIA2 + 0x8
#define VIA2_T2CH VIA2 + 0x9
#define VIA2_SR VIA2 + 0xA
#define VIA2_ACR VIA2 + 0xB
#define VIA2_PCR VIA2 + 0xC
#define VIA2_IFR VIA2 + 0xD
#define VIA2_IER VIA2 + 0xE

// ACIA 0 registers
#define ACIA0_DATA 0xC000
#define ACIA0_STATUS 0xC001
#define ACIA0_COMMAND 0xC002
#define ACIA0_CONTROL 0xC003

// ACIA 1 registers
#define ACIA1_DATA 0xC080
#define ACIA1_STATUS 0xC081
#define ACIA1_COMMAND 0xC082
#define ACIA1_CONTROL 0xC083

// ACIA flags
#define ACIA_READY_RX 0x08
#define ACIA_READY_TX 0x10

// Send a dummy value to the ACIA STATUS register to reset it
#define ACIA_RESET 0x00

// RTS low, transmit interrupt disabled, receiver interrupt disabled, DTR low
#define ACIA_COMMAND_INIT 0x0B

// Baudrate 1/16 oscillator, 8 data bits, 1 stop bit
#define ACIA_CONTROL_BAUD 0b00010000

// Baudrate 9600, 8 data bits, 1 stop bit
#define ACIA_CONTROL_BAUD_9600 0b00011110

// Useful ASCII characters
#define ASCII_NEWLINE 0x0A
#define ASCII_RETURN 0x0D
#define ASCII_ZERO 0x00
#define ASCII_ESC 0x1B

// CH376S Registers
// #define CH376S_DATA 0x8400
// #define CH376S_COMMAND 0x8401

// VGA Card
#define VGA0 0x8000

#define VGA_TEXTMODE_DATA VGA0
#define VGA_TEXTMODE_CLEAR_COMMAND VGA0 + 1
#define VGA_TEXTMODE_SET_COLOR_COMMAND VGA0 + 2

#define VGA_FB_DATA VGA0
#define VGA_FB_SWAP_DATA 0x40

// Utilities
#define MEM(address) (*(volatile unsigned char *)(address))

// === String formatting functions === //
void print_string_bin(char *str, uint8_t max);

#endif
