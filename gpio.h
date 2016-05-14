/*****************************************/
/* GPIO functions header for nrf library */
/*   Platform dependent GPIO functions   */
/*****************************************/



extern void gpio_init(void);
extern void gpio_set_csn(int);
extern void gpio_set_ce(int);
extern int  gpio_read_irq(void);
