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
    // TODO: Add a timeout
    while (!ch376s_has_interrupt())
    {
    }

    ch376s_send_command(CH376S_CMD_GET_STATUS);

    return ch376s_get_byte();
}

bool usb_open_file()
{
    ch376s_send_command(CH376S_CMD_FILE_OPEN);
    return (usb_check_for_interrupts() == CH376S_USB_INT_SUCCESS);
}

uint16_t usb_read_file(uint8_t *buffer, uint16_t max_length)
{
    uint16_t total_bytes_read = 0;
    uint8_t bytes_to_read = 0;
    uint8_t i = 0;
    uint8_t interrupt_response_code = 0;
    bool done = false;

    printf("Reading file contents\r\n");

    ch376s_send_command(CH376S_CMD_BYTE_READ);
    ch376s_send_byte(max_length & 0xFF);
    ch376s_send_byte((max_length & 0xFF00) >> 8);

    while (!done)
    {
        interrupt_response_code = usb_check_for_interrupts();

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
            printf("No more data\r\n");
            done = true;
        }
        else
        {
            printf("Cannot read file. Code: %02X\r\n", interrupt_response_code);
            done = true;
        }
    }

    // Add a trailing zero to the buffer
    buffer[total_bytes_read] = 0;

    return total_bytes_read;
}

int main()
{
    char file_name[14];
    uint16_t file_size = 0;

    usb_reset_module();
    usb_check_exists();
    usb_set_host_mode();

    printf("Enter file name: ");

    readline(file_name);

    printf("\r\n");

    usb_set_file_name(file_name);

    if (!usb_open_file())
    {
        printf("Could not open file\r\n");
        return 1;
    }

    file_size = usb_read_file((uint8_t *)0x5000, 0x2000);

    printf("Read %d bytes\r\n", file_size);

    printf("File contents: %s\r\n", (uint8_t *)0x5000);

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