#include <string.h>
#include <stdint.h>
#include <peekpoke.h>
#include "herring.h"
#include "serial.h"
#include "usb.h"

void ls();
void cat(char *file_name);

void delay(uint16_t x)
{
    uint16_t i = 0;

    while (i < x)
    {
        i++;
    }
}

int main()
{
    // Set up second serial port
    POKE(ACIA1_STATUS, ACIA_RESET);
    POKE(ACIA1_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA1_CONTROL, ACIA_CONTROL_BAUD_9600);

    usb_send_command(CMD_RESET_ALL);

    delay(0xFFF0);

    usb_send_command(CMD_CHECK_EXIST);
    usb_send_byte(0x01);

    if (usb_get_byte() != 0xFE)
    {
        print_line("USB ping failed.");
        return 1;
    }

    // print_line("Set USB host mode");
    usb_send_command(CMD_SET_MODE);
    usb_send_byte(USB_HOST_MODE);

    ls();

    cat("WORDS.TXT");

    print_line("Exiting");

    return 0;
}

void ls()
{
    uint8_t i;
    uint8_t in;
    uint8_t next_status = 0;
    uint8_t data_length = 0;
    char fat_file_info[32] = {0};
    char file_name[9];
    char file_ext[4];

    usb_send_command(CMD_SET_FILENAME);
    usb_send_string("*");
    usb_get_byte();

    // print_line("Open file");
    usb_send_command(CMD_FILE_OPEN);
    in = usb_get_byte();

    if (in == USB_DISK_READ)
    {
        // read FAT info

        while (next_status != USB_FILE_NOT_FOUND)
        {

            usb_send_command(CMD_READ_USB_DATA0);
            data_length = usb_get_byte();
            // puts("data length: ");
            // print_hex(file_length);
            // print_line(0);

            for (i = 0; i < data_length; i++)
            {
                fat_file_info[i] = usb_get_byte();
            }

            // Parse out the file name and size from the FAT info
            strncpy(file_name, fat_file_info, (strchr(fat_file_info, ' ') - fat_file_info));
            strncpy(file_ext, &fat_file_info[8], 3);

            puts(file_name);
            putc('.');
            puts(file_ext);
            putc(' ');
            print_hex(fat_file_info[31]);
            print_hex(fat_file_info[30]);
            print_hex(fat_file_info[29]);
            print_hex(fat_file_info[28]);

            print_line(0);

            // enumerate next file until file not found
            usb_send_command(CMD_NEXT_FILE);
            next_status = usb_get_byte();
        }
    }

    print_line(0);

    // print_line("Close file");
    usb_send_command(CMD_FILE_CLOSE);
    usb_send_byte(0x00);
}

void cat(char *file_name)
{
    uint8_t in = 0;
    uint8_t i = 0;

    uint8_t file_buffer[0x255] = {0};
    uint8_t file_index = 0;
    uint8_t file_length;

    // Set filename
    // print_line("Set filename");
    usb_send_command(CMD_SET_FILENAME);
    usb_send_string(file_name);
    usb_get_byte(); // Read the response and ignore it

    // Open file
    // print_line("Open file");
    usb_send_command(CMD_FILE_OPEN);
    usb_get_byte();

    // Read the file content
    // print_line("Read file");
    usb_send_command(CMD_BYTE_READ);
    usb_send_byte(254); // request up to 254 bytes
    usb_send_byte(0x00);

    in = usb_get_byte();

    if (in == USB_DISK_READ)
    {
        // read the file data
        usb_send_command(CMD_READ_USB_DATA0);

        // read the data length
        file_length = usb_get_byte();

        for (i = 0; i < file_length; i++)
        {
            file_buffer[file_index] = usb_get_byte();
            file_index++;
        }
    }

    // print_line("Close file");
    usb_send_command(CMD_FILE_CLOSE);
    usb_send_byte(0x00);
    usb_get_byte();

    print_line("\r\nFile content: ");
    puts(file_buffer);

    print_line(0);
}
