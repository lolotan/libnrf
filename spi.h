/****************************************/
/* SPI functions header for nrf library */
/*   Platform dependant SPI functions   */
/****************************************/

#define SPI_SUCCESS 0
#define SPI_ERROR   -1

extern int spi_init();
extern int spi_send_command(char, char *);
extern int spi_command_write(char, char *, int, char *);
extern int spi_command_read(char, char *, int, char *);
