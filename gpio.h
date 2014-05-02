/*****************************************/
/* GPIO functions header for nrf library */
/*    Platform dependant - RPI version   */
/*****************************************/

#define GPIO_ON		1
#define GPIO_OFF	0

#define GPIO_IRQ	23
#define GPIO_CE		24

void GPIO_Init(void);
void GPIO_SetCSN(int);
void GPIO_SetCE(int);
int  GPIO_ReadIRQ(int);
