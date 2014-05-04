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

// Global SPI setup variables
static int 		fd;
static uint8_t 	spi_bits;
static uint32_t spi_speed;
static uint16_t spi_delay;

// SPI Initialisation function
// Configure the SPI interface according to the nrf specs
// MSBit to LSBit / LSByte to MSByte / 8 bit command set / 0-8Mbps
int  SPI_Init()
{
	int          Ret;	
	uint8_t      spi_mode;
	const char * spi_device = SPI_DEVICE;
	
	spi_bits  = 8;
	spi_speed = 2000000;
	spi_delay = 0;
	spi_mode  = 0;
	
	fd = open(spi_device, O_RDWR);	
	Ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);	
	Ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);	
	Ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);	
	Ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
	Ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
	Ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
	
	return Ret;
}


int SPI_SendCommand(char Command, char * RetStatus)
{
    int Ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)&Command,
		.rx_buf = (unsigned long)RetStatus,
		.len = 1,
		.delay_usecs = spi_delay,
		.speed_hz = spi_speed,
		.bits_per_word = spi_bits,
	};
    Ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return Ret;
}


int SPI_CommandWrite(char Command, char * WriteBuffer, int Length, char * RetStatus)
{
    int Ret;
	struct spi_ioc_transfer SpiCtrl[2];
    memset(SpiCtrl, 0x00, sizeof(SpiCtrl));
	// Command Struct
	SpiCtrl[0].tx_buf = (unsigned long)&Command;
	SpiCtrl[0].rx_buf = (unsigned long)RetStatus;
	SpiCtrl[0].len = 1;
	SpiCtrl[0].delay_usecs = spi_delay;
	SpiCtrl[0].speed_hz = spi_speed;
	SpiCtrl[0].bits_per_word = spi_bits;
	
	// Data Struct
	SpiCtrl[1].tx_buf = (unsigned long)WriteBuffer;
	SpiCtrl[1].rx_buf = (unsigned long)NULL;
	SpiCtrl[1].len = Length;
	SpiCtrl[1].delay_usecs = spi_delay;
	SpiCtrl[1].speed_hz = spi_speed;
	SpiCtrl[1].bits_per_word = spi_bits;
	Ret = ioctl(fd, SPI_IOC_MESSAGE(2), &SpiCtrl);
	return Ret;
}

int	SPI_CommandRead(char Command, char * ReadBuffer, int Length, char * RetStatus)
{
    int Ret;
    struct spi_ioc_transfer SpiCtrl[2];
    memset(SpiCtrl, 0x00, sizeof(SpiCtrl));
	// Command Struct
	SpiCtrl[0].tx_buf = (unsigned long)&Command;
	SpiCtrl[0].rx_buf = (unsigned long)RetStatus;
	SpiCtrl[0].len = 1;
	SpiCtrl[0].delay_usecs = spi_delay;
	SpiCtrl[0].speed_hz = spi_speed;
	SpiCtrl[0].bits_per_word = spi_bits;
	
    // Data Struct
	SpiCtrl[1].tx_buf = (unsigned long)NULL;
	SpiCtrl[1].rx_buf = (unsigned long)ReadBuffer;
	SpiCtrl[1].len = Length;
	SpiCtrl[1].delay_usecs = spi_delay;
	SpiCtrl[1].speed_hz = spi_speed;
	SpiCtrl[1].bits_per_word = spi_bits;
	Ret = ioctl(fd, SPI_IOC_MESSAGE(2), &SpiCtrl);
	return Ret;
}
