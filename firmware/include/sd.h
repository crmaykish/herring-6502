#ifndef SD_H
#define SD_H

#include <stdint.h>
#include <stdbool.h>

// Setup the SD card interface
// Return true on success, false on error
bool sd_init();

// Send a 6 byte command to the SD card
// Returns the first byte of the response
uint8_t sd_command(uint8_t command[6]);

// Read a single block of SD card data into the block pointer
void sd_read(uint32_t block_num, uint8_t *block);

#endif
