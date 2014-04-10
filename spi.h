/****************************************/
/* SPI functions header for nrf library */
/****************************************/

int  SPI_Init();
int  SPI_Read(int, char *, int);
int  SPI_Write(int, char *, int);
char SPI_WriteReadByte(int, char);