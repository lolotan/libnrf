/****************************************/
/* SPI functions header for nrf library */
/*   Platform dependant - RPI version   */
/****************************************/

#define SPI_DEVICE 	"/dev/spidev0.0"

int  SPI_Init();
int  SPI_Read(char *, int);
int  SPI_Write(char *, int);
int  SPI_WriteRead(char *, char *, int);
