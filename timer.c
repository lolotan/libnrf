	/************************************/
/* Timing functions for nrf library */
/************************************/
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ST_BASE (0x20003000)
#define TIMER_OFFSET (4)
#define BLOCK_SIZE (4*1024)

void *st_base; // byte ptr to simplify offset math

void TIMER_Init(void)
{
	int fd;
    // get access to system core memory
    if (-1 == (fd = open("/dev/mem", O_RDONLY))) {
        fprintf(stderr, "open() failed.\n");
        exit(-1);
    }
 
    // map a specific page into process's address space
    if (MAP_FAILED == (st_base = mmap(NULL, BLOCK_SIZE,
                        PROT_READ, MAP_SHARED, fd, ST_BASE))) {
        fprintf(stderr, "mmap() failed.\n");
        exit(-1);
    }
	close(fd); //No need to keep fd open after mmap
}

void TIMER_RPI_Timer(int usvalue)
{
	long long int *timer;
	long long int start;
	
	timer = (long long int *)((char *)st_base + TIMER_OFFSET);
	start = *timer;
	while (*timer - start < usvalue);
}

void TIMER_Wait_us(int usvalue)
{
	if (usvalue < 100)
		TIMER_RPI_Timer(usvalue);	// Special system timer sleep
	else
		usleep(usvalue);			// Linux usleep standard function
}
