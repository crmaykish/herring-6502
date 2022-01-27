#include <stdio.h>
#include <string.h>
#include <peekpoke.h>

#include "ch376s.h"
#include "herring.h"
#include "delay.h"

static char getc1();
static void putc1(uint8_t c);
static void puts1(const uint8_t *s);

void ch376s_send_command(uint8_t command)
{
    putc1(0x57);
    putc1(0xAB);
    putc1(command);
}

void ch376s_send_byte(uint8_t b)
{
    putc1(b);
}

void ch376s_send_string(char *s)
{
    puts1(s);
    putc1(0x00);
}

uint8_t ch376s_get_byte()
{
    return getc1();
}

void ch376s_init()
{
    // Set up ACIA at 115200 baud
    POKE(ACIA1_STATUS, ACIA_RESET);
    POKE(ACIA1_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA1_CONTROL, ACIA_CONTROL_BAUD);

    // Reset CH376S module
    ch376s_send_command(CH376S_CMD_RESET_ALL);
    delay(10);

    // Set to USB host mode
    ch376s_send_command(CH376S_CMD_SET_MODE);
    ch376s_send_byte(CH376S_USB_HOST_MODE);
}

size_t ch376s_file_read(char *filename, uint8_t *buffer, size_t max_length)
{
    uint32_t total_bytes_read = 0;
    uint8_t bytes_to_read = 0;
    uint8_t i = 0;
    uint8_t response = 0;
    bool chunk_done = false;
    bool file_done = false;

    // Set file name
    ch376s_send_command(CH376S_CMD_SET_FILENAME);
    ch376s_send_string(filename);

    ch376s_get_byte();

    // Open file
    ch376s_send_command(CH376S_CMD_FILE_OPEN);

    response = ch376s_get_byte();

    if (response != CH376S_USB_INT_SUCCESS)
    {
        printf("Failed to open file: %02X\r\n", response);
    }

    while (!file_done)
    {
        chunk_done = false;

        ch376s_send_command(CH376S_CMD_BYTE_READ);
        ch376s_send_byte(0xFF);
        ch376s_send_byte(0x00);

        response = ch376s_get_byte();

        if (response == CH376S_USB_INT_SUCCESS)
        {
            chunk_done = true;
            file_done = true;
        }

        // printf("Reading chunk\r\n");

        while (!chunk_done)
        {
            if (response == CH376S_USB_INT_DISK_READ)
            {
                // File data is available, get the number of bytes to read
                ch376s_send_command(CH376S_CMD_READ_USB_DATA0);
                bytes_to_read = ch376s_get_byte();

                if ((total_bytes_read + bytes_to_read) >= max_length)
                {
                    // If there are more bytes available than the max_length allows, only read bytes up to the limit
                    bytes_to_read = max_length - total_bytes_read;

                    // Break early
                    chunk_done = true;
                    file_done = true;
                }

                for (i = 0; i < bytes_to_read; i++)
                {
                    // Read each byte individually and store them in the buffer
                    buffer[total_bytes_read] = ch376s_get_byte();
                    total_bytes_read++;
                }

                ch376s_send_command(CH376S_CMD_BYTE_RD_GO);
                response = ch376s_get_byte();
            }
            else if (response == CH376S_USB_INT_SUCCESS)
            {
                // No more file data available
                chunk_done = true;
            }
            else
            {
                printf("Could not read file. Code: %02X\r\n", response);
                chunk_done = true;
                file_done = true;
            }
        }
    }

    buffer[total_bytes_read] = 0;

    return total_bytes_read;
}

void ch376s_file_list()
{
    uint8_t i;
    uint8_t in;
    uint8_t next_status = 0;
    uint8_t data_length = 0;
    uint32_t file_size = 0;
    char fat_file_info[32] = {0};
    char file_name[9];
    char file_ext[4];

    printf("Listing root folder...\r\n");
    ch376s_send_command(CH376S_CMD_SET_FILENAME);
    ch376s_send_string("*");
    ch376s_get_byte();

    ch376s_send_command(CH376S_CMD_FILE_OPEN);
    in = ch376s_get_byte();

    if (in == CH376S_USB_INT_DISK_READ)
    {
        // read FAT info

        while (next_status != CH376S_ERR_MISS_FILE)
        {

            ch376s_send_command(CH376S_CMD_READ_USB_DATA0);
            data_length = ch376s_get_byte();

            for (i = 0; i < data_length; i++)
            {
                fat_file_info[i] = ch376s_get_byte();
            }

            // Parse out the file name and size from the FAT info
            strncpy(file_name, fat_file_info, (strchr(fat_file_info, ' ') - fat_file_info));
            strncpy(file_ext, &fat_file_info[8], 3);

            file_size = 0;

            file_size += ((uint32_t)(fat_file_info[31]) << 24);
            file_size += ((uint32_t)(fat_file_info[30]) << 16);
            file_size += ((uint32_t)(fat_file_info[29]) << 8);
            file_size += ((uint32_t)(fat_file_info[28]));

            printf("%s.%s    %ld\r\n", file_name, file_ext, file_size);

            memset(file_name, 0, 9);
            memset(file_ext, 0, 4);

            // enumerate next file until file not found
            ch376s_send_command(CH376S_CMD_NEXT_FILE);
            next_status = ch376s_get_byte();
        }
    }
    else
    {
        printf("Could not get file list\r\n");
    }

    printf("\r\n");

    ch376s_send_command(CH376S_CMD_FILE_CLOSE);
    ch376s_send_byte(0x00);
}

static char getc1()
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return PEEK(ACIA1_DATA);
}

static void putc1(uint8_t c)
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_TX) == 0)
    {
    }

    POKE(ACIA1_DATA, c);
}

static void puts1(const uint8_t *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        putc1(s[i]);
        ++i;
    }
}