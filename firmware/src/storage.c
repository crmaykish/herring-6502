#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "herring.h"
#include "serial.h"
#include "usb.h"
#include "delay.h"

uint8_t readline(char *buffer);

void usb_reset_module()
{
    printf("Resetting CH376S USB module... ");
    usb_send_command(CMD_RESET_ALL);

    delay(10);

    printf("Done!\r\n");
}

void usb_check_exists();

void usb_set_host_mode()
{
    printf("Setting USB Host Mode...\r\n");

    usb_send_command(CMD_SET_MODE);
    usb_send_byte(USB_HOST_MODE);

    delay(1);

    printf("response: %02X\r\n", usb_get_byte());
}

void usb_set_file_name(char *filename)
{
    printf("Set file name: %s\r\n", filename);
    usb_send_command(CMD_SET_FILENAME);
    usb_send_string(filename);
}

uint8_t usb_check_for_interrupts()
{
    uint8_t response = 0;

    usb_wait_for_interrupt();

    usb_send_command(CMD_GET_STATUS);

    response = usb_get_byte();

    // printf("int response: %02X\r\n", response);

    return response;
}

void usb_open_file()
{
    printf("open file\r\n");
    usb_send_command(CMD_FILE_OPEN);
    usb_check_for_interrupts();
}

void usb_read_file()
{
    uint8_t length = 0;
    uint16_t i = 0;
    char buffer[256];
    bool done = false;

    uint8_t int_resp = 0;

    printf("Reading file contents\r\n");

    usb_send_command(CMD_BYTE_READ);
    usb_send_byte(0x00);
    usb_send_byte(0x20);

    while (!done)
    {
        int_resp = usb_check_for_interrupts();

        if (int_resp == USB_DISK_READ)
        {
            usb_send_command(CMD_READ_USB_DATA0);

            length = usb_get_byte();

            for (i = 0; i < length; i++)
            {
                usb_send_command(CMD_BYTE_RD_GO);
                buffer[i] = usb_get_byte();
            }

            buffer[i] = 0;

            serial_puts(buffer);
        }
        else if (int_resp == USB_SUCCESS_NO_DATA)
        {
            printf("NO DATA\r\n");
            done = true;
        }
    }

    printf("\r\nDone!\r\n");
}

int main()
{
    char file_name[14];

    usb_reset_module();
    usb_check_exists();
    usb_set_host_mode();

    printf("Enter file name: ");

    readline(file_name);

    printf("\r\n");

    usb_set_file_name(file_name);
    usb_open_file();
    usb_read_file();

    printf("Exiting.\r\n");

    return 0;
}

void usb_check_exists()
{
    uint8_t usb_response = 0;

    printf("Checking for CH376S USB module... ");
    usb_send_command(CMD_CHECK_EXIST);
    usb_send_byte(0x55);

    usb_response = usb_get_byte();

    if (usb_response == 0xAA)
    {
        printf("Done!\r\n");
    }
    else
    {
        printf("\r\nError: Could not find CH376S. Response: %02X\r\n", usb_response);
    }
}

uint8_t readline(char *buffer)
{
    uint8_t count = 0;
    uint8_t in = serial_getc();

    while (in != '\n' && in != '\r')
    {
        // Character is printable ASCII
        if (in >= 0x20 && in < 0x7F)
        {
            serial_putc(in);

            buffer[count] = in;
            count++;
        }

        in = serial_getc();
    }

    buffer[count] = 0;

    return count;
}