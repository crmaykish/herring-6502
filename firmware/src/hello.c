#include <stdio.h>
#include <stdint.h>
#include "serial.h"
#include "herring.h"
#include "sd.h"

static uint8_t block[512] = {0};

void disk_info(uint8_t b[512])
{
    char *oem = &b[3];
    uint16_t bytes_per_sector = (b[0x0C] << 8) | b[0x0B];
    uint8_t sectors_per_cluster = b[0x0D];
    uint16_t reserved_sectors = (b[0x0F] << 8) | b[0x0E];
    uint16_t sectors_per_fat = (b[0x17] << 8) | b[0x16];
    uint32_t number_of_sectors = ((uint32_t)b[0x21] << 24) | ((uint32_t)b[0x21] << 16) | ((uint32_t)b[0x21] << 8) | b[0x20];
    char *filesystem_type = &b[0x36];

    printf("Disk Info:\r\n");

    printf("OEM: %.8s\r\n", oem);
    printf("Bytes per sector: %d\r\n", bytes_per_sector);
    printf("Sectors per cluster: %d\r\n", sectors_per_cluster);
    printf("Reserved sectors: %d\r\n", reserved_sectors);
    printf("Sectors per FAT: %d\r\n", sectors_per_fat);
    printf("Number of sectors: %d\r\n", number_of_sectors);
    printf("Filesystem type: %.8s\r\n", filesystem_type);

    sd_read(reserved_sectors, block);
}

int main()
{
    int j;

    if (!sd_init())
    {
        printf("SD init failed\r\n");
        return 1;
    }

    sd_read(0, block);

    disk_info(block);

    printf("first fat:\r\n");

    for (j = 0; j < 512; j++)
    {
        printf("%02X ", block[j]);
    }

    return 0;
}
