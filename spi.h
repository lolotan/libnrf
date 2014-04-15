/****************************************/
/* SPI functions header for nrf library */
/*   Platform dependant - RPI version   */
/****************************************/

int  SPI_Init();
int  SPI_Read(char *, int);
int  SPI_Write(char *, int);
int  SPI_WriteRead(char *, char *, int);