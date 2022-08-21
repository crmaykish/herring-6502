#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "herring.h"

// The register containing the GPIO pins used for SPI
#define SPI_IO_PORT VIA2_PORTB
#define SPI_IO_DDR VIA2_DDRB

// Define the pin numbers relative to the IO_PORT register
#define SCLK 4
#define MISO 5
#define MOSI 6
#define CS 7

#define SPI_IO_MASK ((1 << CS) | (1 << MOSI) | (1 << SCLK))

#define SPI_EMPTY 0xFF

#define SPI_RETRY_LIMIT 10

// GPIO control
void gpio_put(uint8_t pin, bool val);
bool gpio_get(uint8_t pin);

// Set up the GPIO pins needed for SPI
void spi_init();

// Send and receive a single byte over SPI
uint8_t spi_transfer(uint8_t b);

#endif