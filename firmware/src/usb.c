#include <stdint.h>
#include <peekpoke.h>
#include "herring.h"
#include "serial.h"

#define CMD_SET_BAUDRATE 0x02
#define CMD_RESET_ALL 0x05
#define CMD_CHECK_EXIST 0x06
#define CMD_SET_MODE 0x15
#define CMD_READ_USB_DATA0 0x27
#define CMD_SET_FILENAME 0x2F
#define CMD_DISK_CONNECTED 0x30
#define CMD_FILE_OPEN 0x32
#define CMD_FILE_CLOSE 0x36
#define CMD_BYTE_READ 0x3A

void delay(uint16_t x)
{
    uint16_t i = 0;

    while (i < x)
    {
        i++;
    }
}

char getc1()
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_RX) == 0)
    {
    }

    return PEEK(ACIA1_DATA);
}

void putc1(uint8_t c)
{
    while ((PEEK(ACIA1_STATUS) & ACIA_READY_TX) == 0)
    {
    }

    POKE(ACIA1_DATA, c);
}

void puts1(const uint8_t *s)
{
    uint8_t i = 0;

    while (s[i] != ASCII_ZERO)
    {
        putc1(s[i]);
        ++i;
    }
}

void send_command(uint8_t b)
{
    putc1(0x57);
    putc1(0xAB);
    putc1(b);
}

int main()
{
    uint8_t in = 0;
    uint8_t i = 0;
    uint8_t len = 0;

    // Set up second serial port
    POKE(ACIA1_STATUS, ACIA_RESET);
    POKE(ACIA1_COMMAND, ACIA_COMMAND_INIT);
    POKE(ACIA1_CONTROL, ACIA_CONTROL_BAUD_9600);

    print_line("starting");

    print_line("Resetting USB");
    send_command(CMD_RESET_ALL);

    delay(0xFFF0);

    // TODO: set baudrate here?

    print_line("Pinging USB");

    send_command(CMD_CHECK_EXIST);
    putc1(0x01);

    in = getc1();

    if (in == 0xFE)
    {
        print_line("ping success");
    }
    else
    {
        print_line("ping failed");
    }

    print_line("Setting mode");

    send_command(CMD_SET_MODE);
    putc1(0x05); // USB host mode, no SOF

    delay(0x8000);

    // Set filename
    send_command(CMD_SET_FILENAME);
    puts1("WORDS.TXT");
    putc1(0x00);
    in = getc1();

    puts("set filename result: ");
    print_hex(in);
    print_line(0);

    // Open file
    send_command(CMD_FILE_OPEN);
    in = getc1();

    puts("file open result: ");
    print_hex(in);
    print_line(0);

    // Read file

    // request byte read

    send_command(CMD_BYTE_READ);
    putc1(0xFE); // request up to 254 bytes?
    putc1(0x00);

    in = getc1();
    puts("byte red req response: ");
    print_hex(in);
    print_line(0);

    if (in == 0x1D)
    {
        // read the file data
        send_command(CMD_READ_USB_DATA0);

        // read the data length
        len = getc1();

        puts("file length: ");
        print_hex(len);
        print_line(0);

        // for data length, read bytes

        print_line("File content: ");

        while (i < len)
        {
            in = getc1();
            putc(in);

            POKE(0x7000 + i, in);

            i++;
        }
    }

    print_line(0);

    send_command(CMD_FILE_CLOSE);
    putc1(0x00);

    in = getc1();
    puts("File close response: ");
    print_hex(in);
    print_line(0);

    print_line("Exiting");

    return 0;
}