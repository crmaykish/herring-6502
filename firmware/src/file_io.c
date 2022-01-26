#include <stdbool.h>
#include <stdio.h>
#include <peekpoke.h>

#include "file_io.h"
#include "ch376s.h"
#include "delay.h"
#include "serial.h"

#define CHECK_EXIST_REQUEST_VAL 0x55
#define CHECK_EXIST_RESPONSE_VAL 0xAA

static uint8_t check_for_interrupts()
{
    // TODO: Add a timeout
    while (!ch376s_has_interrupt())
    {
    }

    ch376s_send_command(CH376S_CMD_GET_STATUS);

    return ch376s_get_byte();
}

void usb_reset_module()
{
    printf("Resetting CH376S USB module");
    ch376s_send_command(CH376S_CMD_RESET_ALL);

    delay(30);

    // ch376s_get_byte();

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

    printf("\r\nresponse: %02X\r\n", ch376s_get_byte());
}

bool file_io_init()
{
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
    uint8_t interrupt_response_code = 0;
    bool done = false;

    // Set file name
    ch376s_send_command(CH376S_CMD_SET_FILENAME);
    ch376s_send_string(filename);

    // Open file
    ch376s_send_command(CH376S_CMD_FILE_OPEN);

    interrupt_response_code = check_for_interrupts();

    if (interrupt_response_code != CH376S_USB_INT_SUCCESS)
    {
        printf("Failed to open file: %02X\r\n", interrupt_response_code);
    }

    // Read file contents

    ch376s_send_command(CH376S_CMD_BYTE_READ);
    ch376s_send_byte(max_length & 0xFF);
    ch376s_send_byte((max_length & 0xFF00) >> 8);

    while (!done)
    {
        interrupt_response_code = check_for_interrupts();

        if (interrupt_response_code == CH376S_USB_INT_DISK_READ)
        {
            ch376s_send_command(CH376S_CMD_READ_USB_DATA0);

            bytes_to_read = ch376s_get_byte();

            for (i = 0; i < bytes_to_read; i++)
            {
                ch376s_send_command(CH376S_CMD_BYTE_RD_GO);

                buffer[total_bytes_read] = ch376s_get_byte();

                total_bytes_read++;
            }
        }
        else if (interrupt_response_code == CH376S_USB_INT_SUCCESS)
        {
            done = true;
        }
        else
        {
            done = true;
        }
    }

    // Add a trailing zero to the buffer
    buffer[total_bytes_read] = 0;

    return total_bytes_read;
}
