#ifndef USB_H
#define USB_H

#include <stdint.h>

#define USB_SUCCESS 0x51
#define USB_SUCCESS_NO_DATA 0x14
#define USB_DISK_READ 0x1D
#define USB_FILE_NOT_FOUND 0x42

#define CMD_SET_BAUDRATE 0x02
#define CMD_RESET_ALL 0x05
#define CMD_CHECK_EXIST 0x06
#define CMD_SET_MODE 0x15
#define CMD_READ_USB_DATA0 0x27
#define CMD_SET_FILENAME 0x2F
#define CMD_DISK_CONNECTED 0x30
#define CMD_FILE_OPEN 0x32
#define CMD_NEXT_FILE 0x33
#define CMD_FILE_CLOSE 0x36
#define CMD_DIR_INFO 0x37
#define CMD_BYTE_READ 0x3A

#define USB_HOST_MODE 0x05

void usb_send_command(uint8_t command);
void usb_send_byte(uint8_t b);
void usb_send_string(char *s);

uint8_t usb_get_byte();

#endif