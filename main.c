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
    char Status;
    int  Ret;
    char TestBuffer[32];
    const char * TestStr = "Hello World !";
    
    memset(TestBuffer, 0x00, sizeof(TestBuffer));

	printf("*** Test nrf lib ***\n");
    Ret = NRF_Init();
    printf("NRF Init : return %d\n", Ret);
    
    Ret = NRF_GetStatus(&Status);
    NRF_DisplayStatus(Status);

    Ret = NRF_SetPowerMode(POWER_ON, &Status);

    Ret = NRF_FlushTX(&Status);
    Ret = NRF_FlushRX(&Status);
    Ret = NRF_SetModePTX(&Status);

    memcpy(TestBuffer, TestStr, strlen(TestStr));

    Ret = NRF_WriteTXPayload(TestBuffer, sizeof(TestBuffer), &Status);
    NRF_DisplayStatus(Status);

    NRF_TXPayload();

    Ret = NRF_GetStatus(&Status);
    NRF_DisplayStatus(Status);

    int PacketCount;
    Ret = NRF_GetLostPacketsCount(&PacketCount, &Status);
    printf("Lost Packets count : %d\n", PacketCount);

    Ret = NRF_GetLostRetriesCount(&PacketCount, &Status);
    printf("Lost Packets retries : %d\n", PacketCount);

    Ret = NRF_SetPowerMode(POWER_OFF, &Status);

    return 0;
}
