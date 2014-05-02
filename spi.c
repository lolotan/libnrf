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
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// Global SPI setup variables
static int 		fd;
static uint8_t 	bits;
static uint32_t speed;
static uint16_t delay;

// SPI Initialisation function
// Configure the SPI interface according to the nrf specs
// MSBit to LSBit / LSByte to MSByte / 8 bit command set / 0-8Mbps
int  SPI_Init()
{
	int          Ret;	
	uint8_t      mode;
	const char * spi_device = SPI_DEVICE;
	
	bits  = 0;
	speed = 2000000;
	delay = 0;
	mode  = 0;
	
	fd = open(spi_device, O_RDWR);	
	Ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);	
	Ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);	
	Ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);	
	Ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	Ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	Ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	
	return Ret;
}

int  SPI_Read(char * ReadBuffer, int Length)
{
	int Ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)NULL,
		.rx_buf = (unsigned long)ReadBuffer,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	Ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return Ret;
}

int  SPI_Write(char * WriteBuffer, int Length)
{
	int Ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)WriteBuffer,
		.rx_buf = (unsigned long)NULL,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	Ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return Ret;
}

int  SPI_WriteRead(char * WriteBuffer, char * ReadBuffer, int Length)
{
	int Ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)WriteBuffer,
		.rx_buf = (unsigned long)ReadBuffer,
		.len = Length,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	Ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);	
	return Ret;
}

int SPI_SendCommand(char Command, char * RetStatus)
{
    int Ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)&Command,
		.rx_buf = (unsigned long)RetStatus,
		.len = 1,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
    Ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return Ret;
}


int SPI_CommandWrite(char Command, char * WriteBuffer, int Length, char * RetStatus)
{
    int Ret;
	struct spi_ioc_transfer SpiCtrl[2];
	// Command Struct
	SpiCtrl[0].tx_buf = (unsigned long)&Command;
	SpiCtrl[0].rx_buf = (unsigned long)RetStatus;
	SpiCtrl[0].len = 1;
	SpiCtrl[0].delay_usecs = delay;
	SpiCtrl[0].speed_hz = speed;
	SpiCtrl[0].bits_per_word = bits;
	
	// Data Struct
	SpiCtrl[1].tx_buf = (unsigned long)WriteBuffer;
	SpiCtrl[1].rx_buf = (unsigned long)NULL;
	SpiCtrl[1].len = Length;
	SpiCtrl[1].delay_usecs = delay;
	SpiCtrl[1].speed_hz = speed;
	SpiCtrl[1].bits_per_word = bits;
	Ret = ioctl(fd, SPI_IOC_MESSAGE(2), SpiCtrl);
	return Ret;
}

int	SPI_CommandRead(char Command, char * ReadBuffer, int Length, char * RetStatus)
{
    int Ret;
    struct spi_ioc_transfer SpiCtrl[2];
	// Command Struct
	SpiCtrl[0].tx_buf = (unsigned long)&Command;
	SpiCtrl[0].rx_buf = (unsigned long)RetStatus;
	SpiCtrl[0].len = 1;
	SpiCtrl[0].delay_usecs = delay;
	SpiCtrl[0].speed_hz = speed;
	SpiCtrl[0].bits_per_word = bits;
	
	// Data Struct
	SpiCtrl[1].tx_buf = (unsigned long)NULL;
	SpiCtrl[1].rx_buf = (unsigned long)ReadBuffer;
	SpiCtrl[1].len = Length;
	SpiCtrl[1].delay_usecs = delay;
	SpiCtrl[1].speed_hz = speed;
	SpiCtrl[1].bits_per_word = bits;
	Ret = ioctl(fd, SPI_IOC_MESSAGE(2), SpiCtrl);
	return Ret;
}
