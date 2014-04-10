/*****************************************/
/* GPIO functions header for nrf library */
/*****************************************/

void GPIO_Init(void);
void GPIO_SetCSN(int);
void GPIO_SetCE(int);
int  GPIO_ReadIRQ(int);