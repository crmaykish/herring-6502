#include <stdio.h>
#include <stdint.h>

#include "serial.h"
#include "delay.h"
#include "herring.h"

#define SCLK 0
#define MOSI 1
#define MISO 2
#define CS 3

static uint8_t CMD0[6] = {0x40 + 0, 0x00, 0x00, 0x00, 0x00, 0x95};
static uint8_t CMD8[6] = {0x40 + 8, 0x00, 0x00, 0x01, 0xAA, 0x87};
static uint8_t CMD58[6] = {0x40 + 58, 0x00, 0x00, 0x00, 0x00, 0x01};
static uint8_t CMD55[6] = {0x40 + 55, 0x00, 0x00, 0x00, 0x00, 0x01};
static uint8_t ACMD41[6] = {0x40 + 41, 0x40, 0x00, 0x00, 0x00, 0x01};
static uint8_t CMD17[6] = {0x40 + 17, 0x00, 0x00, 0x00, 0x00, 0x01};

static uint8_t block[512] = {0};

void gpio_put(uint8_t pin, bool val)
{
    if (val)
    {
        MEM(VIA1_PORTA) |= (1 << pin);
    }
    else
    {
        MEM(VIA1_PORTA) &= ~(1 << pin);
    }
}

bool gpio_get(uint8_t pin)
{
    return ((MEM(VIA1_PORTA) & (1 << pin)) > 0);
}

uint8_t spi_read_byte()
{
    uint8_t in = 0;
    int i;

    gpio_put(SCLK, false);

    // delay(1);

    // enable SD card
    gpio_put(CS, false);

    gpio_put(MOSI, true);

    // delay(1);

    for (i = 7; i >= 0; i--)
    {
        gpio_put(SCLK, true);

        // delay(1);

        if (gpio_get(MISO))
        {
            in |= (1 << i);
        }

        gpio_put(SCLK, false);

        // delay(1);
    }

    // disable SD card
    gpio_put(CS, true);

    return in;
}

void spi_write_byte(uint8_t b)
{
    int i;

    gpio_put(SCLK, false);
    // delay(1);

    gpio_put(CS, false);
    // delay(1);

    for (i = 7; i >= 0; i--)
    {
        if (b & (1 << i))
        {
            gpio_put(MOSI, true);
        }
        else
        {
            gpio_put(MOSI, false);
        }

        // delay(1);

        gpio_put(SCLK, true);

        // delay(1);

        gpio_put(SCLK, false);
    }

    // delay(1);

    gpio_put(CS, true);

    // delay(1);
}

void sd_send_command(uint8_t command[6])
{
    int i;
    for (i = 0; i < 6; i++)
    {
        spi_write_byte(command[i]);
    }
}

uint8_t sd_wait_result()
{
    uint8_t response = spi_read_byte();
    int count = 0;

    while (response == 0xFF && count < 10)
    {
        printf("waiting for response\r\n");
        // delay(10);

        response = spi_read_byte();
        count++;
    }

    return response;
}

void sd_read_block(uint32_t block_num, uint8_t *block_data)
{
    uint8_t command[6] = {0x40 + 17, 0, 0, 0, 0, 0x01};
    uint8_t result;
    int i;

    command[4] = (block_num & 0xFF);
    command[3] = ((block_num & 0xFF00) >> 8);

    sd_send_command(command);
    result = sd_wait_result();

    printf("CMD17 response: %X\r\n", result);

    if (result != 0x00)
    {
        printf("CMD17 failed\r\n");
        return;
    }

    while (result != 0xFE)
    {
        result = sd_wait_result();
    }

    printf("ready to read block\r\n");

    for (i = 0; i < 512; i++)
    {
        block_data[i] = spi_read_byte();
    }

    // read the 16 bit CRC
    spi_read_byte();
    spi_read_byte();

    printf("read block: %d\r\n", block_num);
}

void disk_info(uint8_t b[512])
{
    char *oem = &b[3];
    uint16_t bytes_per_sector = (b[0x0C] << 8) | b[0x0B];
    uint8_t sectors_per_cluster = b[0x0D];
    uint16_t reserved_sectors = (b[0x0F] << 8) | b[0x0E];
    uint16_t sectors_per_fat = (b[0x17] << 8) | b[0x16];
    uint32_t number_of_sectors = ((uint32_t)b[0x21] << 24) | ((uint32_t)b[0x21] << 16) | ((uint32_t)b[0x21] << 8) | b[0x20];
    char *filesystem_type = &b[0x36];

    printf("\r\nDisk Info:\r\n");

    printf("OEM: %.8s\r\n", oem);
    printf("Bytes per sector: %d\r\n", bytes_per_sector);
    printf("Sectors per cluster: %d\r\n", sectors_per_cluster);
    printf("Reserved sectors: %d\r\n", reserved_sectors);
    printf("Sectors per FAT: %d\r\n", sectors_per_fat);
    printf("Number of sectors: %d\r\n", number_of_sectors);
    printf("Filesystem type: %.8s\r\n", filesystem_type);

    sd_read_block(reserved_sectors, block);
}

int main()
{
    int i, j;
    uint8_t first_byte;
    uint8_t response;
    uint32_t cmd58parsed;
    bool init;

    printf("SD card test\r\n");

    MEM(VIA1_DDRA) = 0b111111011;

    // delay(10);

    gpio_put(CS, true);
    gpio_put(MOSI, true);

    printf("Warming up SD card...\r\n");

    for (i = 0; i < 80; i++)
    {
        gpio_put(SCLK, true);
        // delay(1);
        gpio_put(SCLK, false);
        // delay(1);
    }

    first_byte = spi_read_byte();

    printf("Read first byte: %02X\r\n", first_byte);

    sd_send_command(CMD0);
    response = sd_wait_result();

    printf("CMD0 response: %02X\r\n", response);

    if (response != 0x01)
    {
        printf("CMD0 failed\r\n");
        return 1;
    }

    sd_send_command(CMD8);
    response = sd_wait_result();

    printf("CMD8 response: %02X\r\n", response);

    spi_read_byte();
    spi_read_byte();
    spi_read_byte();
    spi_read_byte();

    sd_send_command(CMD58);
    response = sd_wait_result();
    printf("CMD58 response: %02X\r\n", response);

    spi_read_byte();
    spi_read_byte();
    spi_read_byte();
    spi_read_byte();

    init = false;

    while (!init)
    {
        printf("try init...\r\n");
        sd_send_command(CMD55);
        response = sd_wait_result();

        printf("CMD55 response: %02X\r\n", response);

        sd_send_command(ACMD41);
        response = sd_wait_result();

        printf("ACMD41 response: %02X\r\n", response);

        if (response == 0x00)
        {
            init = true;
        }

        // delay(10);
    }

    printf("Init complete\r\n");

    // for (i = 0; i < 10; i++)
    // {
    // printf("BLOCK: %d\r\n", i);
    // sd_read_block(i, block);
    sd_read_block(0, block);

    // for (j = 0; j < 512; j++)
    // {
    //     printf("%02X ", block[j]);
    // }
    // }

    // printf("\r\nBLOCK TEXT: %s", (char *)block);

    disk_info(block);

    printf("first fat:\r\n");

    for (j = 0; j < 512; j++)
    {
        printf("%02X ", block[j]);
    }

    return 0;
}
