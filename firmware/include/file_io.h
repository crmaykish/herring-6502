#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdint.h>

/**
 * @brief Initialize the file I/O subsystem
 * 
 * @return true success
 * @return false failure
 */
bool file_io_init();

uint16_t file_read(char *filename, uint8_t *buffer, uint16_t max_length);

#endif