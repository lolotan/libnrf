/************************************/
/*  SPI functions for nrf library   */
/* Platform dependant - RPI version */
/************************************/
#include "spi.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
static int fd; // Global SPI device descriptor
static uint8_t bits;
static uint32_t speed;
static uint16_t delay;

// SPI Initialisation function
// Configure the SPI interface according to the nrf specs
// MSBit to LSBit / LSByte to MSByte / 8 bit command set / 0-8Mbps
int  SPI_Init()
{
	int          ret;	
	uint8_t      mode;
	const char * spi_device = "/dev/spidev0.0";
	
	bits  = 0;
	speed = 500000;
	delay = 0;
	mode  = 0;
	
	fd = open(spi_device, O_RDWR);	
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);	
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);	
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);	
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	
	return ret;
}

int  SPI_Read(char * ReadBuffer, int Length)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)NULL,
		.rx_buf = (unsigned long)ReadBuffer,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return ret;
}

int  SPI_Write(char * WriteBuffer, int Length)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)WriteBuffer,
		.rx_buf = (unsigned long)NULL,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return ret;
}

int  SPI_WriteRead(char * WriteBuffer, char * ReadBuffer, int Length)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)WriteBuffer,
		.rx_buf = (unsigned long)ReadBuffer,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return ret;
}
