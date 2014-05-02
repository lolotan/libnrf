/****************************************/
/* SPI functions header for nrf library */
/*   Platform dependant - RPI version   */
/****************************************/

#define SPI_DEVICE 	"/dev/spidev0.0"

int SPI_Init();
int SPI_SendCommand(char, char *);
int SPI_CommandWrite(char, char *, int, char *);
int	SPI_CommandRead(char, char *, int, char *);
