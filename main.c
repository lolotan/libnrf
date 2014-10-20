/******************************************/
/* Main test program for nrf library test */
/******************************************/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "nrflib.h"
#include "timer.h"

int main(void)
{	
    char TestVal;
    char Status;
    char TestBuffer[32];
    int Ret;
    
    memset(TestBuffer, '*', sizeof(TestBuffer));

	printf("*** Test nrf lib ***\n");
    Ret = NRF_Init();
    printf("NRF Init : return %d\n", Ret);
    
    Ret = NRF_GetStatus(&Status);
    NRF_DisplayStatus(Status);

    Status = 0x00;
    TestVal = 0x00;
    Ret = NRF_WriteRegister(CONFIG, 0x00, &Status);
    
    Ret = NRF_ReadRegister(CONFIG, &TestVal, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetPowerMode(POWER_ON, &Status);
    printf("SetPowerMode register : return %d status %.2X\n", Ret, Status);

    Ret = NRF_ReadRegister(CONFIG, &TestVal, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);

    Ret = NRF_SetModePTX(&Status);
    printf("SetModePTX register : return %d status %.2X\n", Ret, Status);

    Ret = NRF_ReadRegister(CONFIG, &TestVal, &Status);
    printf("Read CONFIG register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetDataRate(DR2MBPS, &Status);
    printf("Test Data Rate : return %d status %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);

    
    Ret = NRF_SetPAControl(PA18DBM, &Status);
    printf("Test PA Control : return %d status %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_SETUP, &TestVal, &Status);
    printf("Read RF_SETUP register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);

    Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);
    NRF_DisplayStatus(Status);
    Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);
	NRF_DisplayStatus(Status);
	Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);
	NRF_DisplayStatus(Status);
	Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);
	NRF_DisplayStatus(Status);

	NRF_FlushTX(&Status);
	printf("Test Flush TX\n");
	Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);
	NRF_DisplayStatus(Status);
    /*
    Ret = NRF_ReadRegister(RF_CH, &TestVal, 1, &Status);
    printf("Read RF_CH register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
    
    Ret = NRF_SetRFChannel(3, &Status);
    printf("Test RF Channel : return %d status %.2X\n", Ret, Status);
    
    Ret = NRF_ReadRegister(RF_CH, &TestVal, 1, &Status);
    printf("Read RF_CH register : return %d value %.2X status %.2X\n", Ret, TestVal, Status);
	*/
    return 0;
}
