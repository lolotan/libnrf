/******************************************/
/* Main test program for nrf library test */
/******************************************/

#include <stdio.h>
#include <unistd.h>
#include "nrflib.h"


int main(void)
{	
    char TestVal;
    char Status;
    int Ret;
    
	printf("*** Test nrf lib ***\n");
    Ret = NRF_Init();
    printf("NRF Init : return %d\n", Ret);
    
    Ret = NRF_GetStatus(&Status);
    printf("Status register : return %d value %.2X\n", Ret, Status);

    Status = 0x00;
    TestVal = 0x00;
    Ret = SPI_CommandWrite(W_REGISTER | CONFIG, &TestVal, 1, &Status);
    TIMER_Wait_us(1600);
    
    Ret = NRF_ReadRegister(CONFIG, &TestVal, 1, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetPowerMode(POWER_ON, &Status);
    printf("SetPowerMode register : return %d status %.2X\n", Ret, Status);
    
    TIMER_Wait_us(1600); // Tpd2stby datasheet p.22

    Ret = NRF_ReadRegister(CONFIG, &TestVal, 1, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetModePRX(&Status);
    printf("SetModePRX register : return %d status %.2X\n", Ret, Status);
    
    TIMER_Wait_us(130); // Tstby2a datasheet p.22

    Ret = NRF_ReadRegister(CONFIG, &TestVal, 1, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    /*
    Ret = NRF_GetStatus(&Status);
    printf("Status register : return %d value %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, 1, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetDataRate(DR250KBPS, &Status);
    printf("Test Data Rate : return %d status %.2X\n", Ret, Status);
    
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, 1, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetPAControl(PA18DBM, &Status);
    printf("Test PA Control : return %d status %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, 1, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    
    Ret = NRF_ReadRegister(RF_CH, &TestVal, 1, &Status);
    printf("Read RF_CH register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetRFChannel(3, &Status);
    printf("Test RF Channel : return %d status %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_CH, &TestVal, 1, &Status);
    printf("Read RF_CH register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
	*/
    return 0;
}
