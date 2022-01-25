#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <peekpoke.h>
#include "herring.h"
#include "serial.h"
#include "usb.h"

// void ls();
// void cat(char *file_name);

void usb_reset_module()
{
    printf("Resetting CH376S USB module... ");
    usb_send_command(CMD_RESET_ALL);

    delay(0x2000);

    printf("Done!\r\n");
}

void usb_check_exists();

void usb_set_host_mode()
{
    printf("Setting USB Host Mode...\r\n");

    usb_send_command(CMD_SET_MODE);
    usb_send_byte(USB_HOST_MODE);

    delay(0x1000);

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

    printf("int response: %02X\r\n", response);

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
    uint8_t i = 0;
    char *buffer = (char *)0x7000;

    printf("read bytes\r\n");
    usb_send_command(CMD_BYTE_READ);
    usb_send_byte(0xFE);
    usb_send_byte(0x00);

    if (usb_check_for_interrupts() == USB_DISK_READ)
    {
        printf("good\r\n");

        usb_send_command(CMD_READ_USB_DATA0);

        length = usb_get_byte();

        printf("length: %d\r\n", length);

        for (i = 0; i < length; i++)
        {
            usb_send_command(CMD_BYTE_RD_GO);
            buffer[i] = usb_get_byte();
        }

        buffer[i] = 0;

        printf("file contents: %s\r\n", buffer);
    }
}

int main()
{
    usb_reset_module();
    usb_check_exists();
    usb_set_host_mode();

    usb_set_file_name("WORDS.TXT");

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