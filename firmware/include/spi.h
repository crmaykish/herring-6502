#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "herring.h"

// The register containing the GPIO pins used for SPI
#define SPI_IO_PORT VIA1_PORTA
#define SPI_IO_DDR VIA1_DDRA

// Define the pin numbers relative to the IO_PORT register
#define SCLK 0
#define MOSI 1
#define MISO 2
#define CS 3

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