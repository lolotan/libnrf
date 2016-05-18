/************************************/
/*  SPI functions for nrf library   */
/* Platform dependant - RPI version */
/************************************/
#include "spi.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE 	"/dev/spidev0.0"

// Global SPI setup variables
static int 		fd;
static uint8_t 	spi_bits;
static uint32_t spi_speed;
static uint16_t spi_delay;

// SPI Initialisation function
// Configure the SPI interface according to the nrf specs
// MSBit to LSBit / LSByte to MSByte / 8 bit command set / 0-8Mbps
int  spi_init()
{
	int          ret;	
	uint8_t      spi_mode;
	const char * spi_device = SPI_DEVICE;
	
	spi_bits  = 8;
	spi_speed = 2000000;
	spi_delay = 0;
	spi_mode  = 0;
	
	fd = open(spi_device, O_RDWR);	
	ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);	
	ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);	
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);	
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	
	return ret;
}


int spi_send_command(char command, char * retstatus)
{
    int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)&command,
		.rx_buf = (unsigned long)retstatus,
		.len = 1,
		.delay_usecs = spi_delay,
		.speed_hz = spi_speed,
		.bits_per_word = spi_bits,
	};
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return ret;
}


int spi_command_write(char command, char * writebuffer, int length, char * retstatus)
{
    int ret;
	struct spi_ioc_transfer spictrl[2];
    memset(spictrl, 0x00, sizeof(spictrl));
	// Command Struct
	spictrl[0].tx_buf = (unsigned long)&command;
	spictrl[0].rx_buf = (unsigned long)retstatus;
	spictrl[0].len = 1;
	spictrl[0].delay_usecs = spi_delay;
	spictrl[0].speed_hz = spi_speed;
	spictrl[0].bits_per_word = spi_bits;
	
	// Data Struct
	spictrl[1].tx_buf = (unsigned long)writebuffer;
	spictrl[1].rx_buf = (unsigned long)NULL;
	spictrl[1].len = length;
	spictrl[1].delay_usecs = spi_delay;
	spictrl[1].speed_hz = spi_speed;
	spictrl[1].bits_per_word = spi_bits;
	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &spictrl);
	return ret;
}

int	spi_command_read(char command, char * readbuffer, int length, char * retstatus)
{
    int ret;
    struct spi_ioc_transfer spictrl[2];
    memset(spictrl, 0x00, sizeof(spictrl));
	// Command Struct
	spictrl[0].tx_buf = (unsigned long)&command;
	spictrl[0].rx_buf = (unsigned long)retstatus;
	spictrl[0].len = 1;
	spictrl[0].delay_usecs = spi_delay;
	spictrl[0].speed_hz = spi_speed;
	spictrl[0].bits_per_word = spi_bits;
	
    // Data Struct
	spictrl[1].tx_buf = (unsigned long)NULL;
	spictrl[1].rx_buf = (unsigned long)readbuffer;
	spictrl[1].len = length;
	spictrl[1].delay_usecs = spi_delay;
	spictrl[1].speed_hz = spi_speed;
	spictrl[1].bits_per_word = spi_bits;
	ret = ioctl(fd, SPI_IOC_MESSAGE(2), &spictrl);
	return ret;
}
