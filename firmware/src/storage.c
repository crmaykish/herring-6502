#include <stdio.h>
#include <stdint.h>

#include "ch376s.h"
#include "serial.h"
#include "delay.h"

#define CHECK_EXIST_REQUEST_VAL 0x55
#define CHECK_EXIST_RESPONSE_VAL 0xAA

uint8_t readline(char *buffer);

void usb_reset_module()
{
    printf("Resetting CH376S USB module... ");
    ch376s_send_command(CH376S_CMD_RESET_ALL);

    delay(10);

    printf("Done!\r\n");
}

void usb_check_exists();

void usb_set_host_mode()
{
    printf("Setting USB Host Mode...\r\n");

    ch376s_send_command(CH376S_CMD_SET_MODE);
    ch376s_send_byte(CH376S_USB_HOST_MODE);

    delay(1);

    printf("response: %02X\r\n", ch376s_get_byte());
}

void usb_set_file_name(char *filename)
{
    printf("Set file name: %s\r\n", filename);
    ch376s_send_command(CH376S_CMD_SET_FILENAME);
    ch376s_send_string(filename);
}

uint8_t usb_check_for_interrupts()
{
    uint8_t response = 0;

    while (!ch376s_has_interrupt())
    {
    }

    ch376s_send_command(CH376S_CMD_GET_STATUS);

    response = ch376s_get_byte();

    // printf("int response: %02X\r\n", response);

    return response;
}

void usb_open_file()
{
    printf("open file\r\n");
    ch376s_send_command(CH376S_CMD_FILE_OPEN);
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

    ch376s_send_command(CH376S_CMD_BYTE_READ);
    ch376s_send_byte(0x00);
    ch376s_send_byte(0x20);

    while (!done)
    {
        int_resp = usb_check_for_interrupts();

        if (int_resp == CH376S_USB_INT_DISK_READ)
        {
            ch376s_send_command(CH376S_CMD_READ_USB_DATA0);

            length = ch376s_get_byte();

            for (i = 0; i < length; i++)
            {
                ch376s_send_command(CH376S_CMD_BYTE_RD_GO);
                buffer[i] = ch376s_get_byte();
            }

            buffer[i] = 0;

            serial_puts(buffer);
        }
        else if (int_resp == CH376S_USB_INT_SUCCESS)
        {
            printf("No more data\r\n");
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