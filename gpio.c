/************************************/
/*  GPIO functions for nrf library  */
/* Platform dependant - RPI version */
/************************************/
#include "gpio.h"

// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define GPIO_ON		1
#define GPIO_OFF	0

#define GPIO_IRQ	23
#define GPIO_CE		24

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0


void gpio_init(void)
{
	// Rpi GPIO Setup - from elinux wiki
	
	/* open /dev/mem */
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
		printf("can't open /dev/mem \n");
		exit(-1);
	}

	/* mmap GPIO */
	gpio_map = mmap(
		NULL,             //Any adddress in our space will do
		BLOCK_SIZE,       //Map length
		PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
		MAP_SHARED,       //Shared with other processes
		mem_fd,           //File to map
		GPIO_BASE         //Offset to GPIO peripheral
	);

	close(mem_fd); //No need to keep mem_fd open after mmap

	if (gpio_map == MAP_FAILED) {
		printf("mmap error %d\n", (int)gpio_map);//errno also set!
		exit(-1);
	}

	// Always use volatile pointer!
	gpio = (volatile unsigned *)gpio_map;

	// Initialize GPIO for CE / IRQ pins (CSN not needed on RPi)
	// Set CE to output mode
	INP_GPIO(GPIO_CE); 
	OUT_GPIO(GPIO_CE);
	
	gpio_set_ce(GPIO_OFF);
}

void gpio_set_csn(int gpiostate)
{
	// CSN not needed 
}

void gpio_set_ce(int gpiostate)
{
	if (gpiostate == GPIO_ON) {
		GPIO_SET = 1<<GPIO_CE;
	} else {
		GPIO_CLR = 1<<GPIO_CE;
	}
}

int gpio_read_irq()
{
	// TODO
	int ret = 0;
	return ret;
}
