#include <stdbool.h>
#include <stdio.h>
#include <peekpoke.h>

#include "herring.h"
#include "file_io.h"
#include "ch376s.h"
#include "delay.h"
#include "serial.h"

#define CHECK_EXIST_REQUEST_VAL 0x55
#define CHECK_EXIST_RESPONSE_VAL 0xAA

#define INTERRUPT_TIMEOUT 100

void usb_reset_module()
{
    printf("Resetting CH376S USB module");
    ch376s_send_command(CH376S_CMD_RESET_ALL);

    delay(10);

    printf(" Done!\r\n");
}

void usb_check_exists()
{
    uint8_t usb_response = 0;

    printf("Checking for CH376S USB module... ");
    ch376s_send_command(CH376S_CMD_CHECK_EXIST);
    ch376s_send_byte(CHECK_EXIST_REQUEST_VAL);

    usb_response = ch376s_get_byte();

    if (usb_response == CHECK_EXIST_RESPONSE_VAL)
    {
        printf("Done!\r\n");
    }
    else
    {
        printf("\r\nError: Could not find CH376S. Response: %02X\r\n", usb_response);
    }
}

void usb_set_host_mode()
{
    printf("Setting USB Host Mode\r\n");
    ch376s_send_command(CH376S_CMD_SET_MODE);
    ch376s_send_byte(CH376S_USB_HOST_MODE);
}

bool file_io_init()
{
    // set up ACIA
    POKE(ACIA1_STATUS, ACIA_RESET);
    POKE(ACIA1_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA1_CONTROL, ACIA_CONTROL_BAUD);

    usb_reset_module();
    usb_check_exists();
    usb_set_host_mode();

    return true;
}

uint16_t file_read(char *filename, uint8_t *buffer, uint16_t max_length)
{
    uint16_t total_bytes_read = 0;
    uint8_t bytes_to_read = 0;
    uint8_t i = 0;
    uint8_t response = 0;
    bool done = false;

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

    // Read up to max_length bytes
    ch376s_send_command(CH376S_CMD_BYTE_READ);
    ch376s_send_byte(max_length & 0xFF);
    ch376s_send_byte((max_length & 0xFF00) >> 8);

    // NOTE: I think this limits reading files up to 64KB
    // How do we continue if the file is larger?

    response = ch376s_get_byte();

    while (!done)
    {
        if (response == CH376S_USB_INT_DISK_READ)
        {
            // File data is available, get the number of bytes to read
            ch376s_send_command(CH376S_CMD_READ_USB_DATA0);
            bytes_to_read = ch376s_get_byte();

            for (i = 0; i < bytes_to_read; i++)
            {
                // Read each byte individually and store them in the buffer
                buffer[total_bytes_read] = ch376s_get_byte();
                total_bytes_read++;
            }
        }
        else if (response == CH376S_USB_INT_SUCCESS)
        {
            // No more file data available
            printf("Done reading file\r\n");
            done = true;
        }
        else
        {
            printf("Could not read file. Code: %02X\r\n", response);
            done = true;
        }

        ch376s_send_command(CH376S_CMD_BYTE_RD_GO);
        response = ch376s_get_byte();
    }

    buffer[total_bytes_read] = 0;

    return total_bytes_read;
}

uint16_t file_read_continue(char *filename, uint8_t *buffer, uint16_t max_length)
{
    uint16_t total_bytes_read = 0;
    uint8_t bytes_to_read = 0;
    uint8_t i = 0;
    uint8_t response = 0;
    bool done = false;

    // // Set file name
    // ch376s_send_command(CH376S_CMD_SET_FILENAME);
    // ch376s_send_string(filename);

    // ch376s_get_byte();

    // // Open file
    // ch376s_send_command(CH376S_CMD_FILE_OPEN);

    // response = ch376s_get_byte();

    // if (response != CH376S_USB_INT_SUCCESS)
    // {
    //     printf("Failed to open file: %02X\r\n", response);
    // }

    // Read up to max_length bytes
    ch376s_send_command(CH376S_CMD_BYTE_READ);
    ch376s_send_byte(max_length & 0xFF);
    ch376s_send_byte((max_length & 0xFF00) >> 8);

    // NOTE: I think this limits reading files up to 64KB
    // How do we continue if the file is larger?

    response = ch376s_get_byte();

    while (!done)
    {
        if (response == CH376S_USB_INT_DISK_READ)
        {
            // File data is available, get the number of bytes to read
            ch376s_send_command(CH376S_CMD_READ_USB_DATA0);
            bytes_to_read = ch376s_get_byte();

            for (i = 0; i < bytes_to_read; i++)
            {
                // Read each byte individually and store them in the buffer
                buffer[total_bytes_read] = ch376s_get_byte();
                total_bytes_read++;
            }
        }
        else if (response == CH376S_USB_INT_SUCCESS)
        {
            // No more file data available
            printf("Done reading file\r\n");
            done = true;
        }
        else
        {
            printf("Could not read file. Code: %02X\r\n", response);
            done = true;
        }

        ch376s_send_command(CH376S_CMD_BYTE_RD_GO);
        response = ch376s_get_byte();
    }

    buffer[total_bytes_read] = 0;

    return total_bytes_read;
}
