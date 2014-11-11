/******************************************/
/* Main test program for nrf library test */
/******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "nrflib.h"
#include "timer.h"

void Test_Send(void)
{
	char Status;
	int  Ret;
	char TestBuffer[32];
	const char * TestStr = "Hello World !";

	printf("SEND TEST\n");

	memset(TestBuffer, 0x00, sizeof(TestBuffer));
	Ret = NRF_SetModePTX(&Status);

	memcpy(TestBuffer, TestStr, strlen(TestStr));

	Ret = NRF_WriteTXPayload(TestBuffer, sizeof(TestBuffer), &Status);

	NRF_TXPayload();
	printf("Payload transmitted\n");
	Ret = NRF_GetStatus(&Status);
	NRF_DisplayStatus(Status);

	int PacketCount;
	Ret = NRF_GetLostPacketsCount(&PacketCount, &Status);
	printf("Lost Packets count : %d\n", PacketCount);

	Ret = NRF_GetLostRetriesCount(&PacketCount, &Status);
	printf("Lost Packets retries : %d\n", PacketCount);
}

void Test_Receive(void)
{
	char Status;
	int  Ret;
	int  DataReceived = 0;
	char TestBuffer[32];

	printf("RECEIVE TEST\n");

	memset(TestBuffer, 0x00, sizeof(TestBuffer));
	Ret = NRF_SetModePRX(&Status);
	NRF_StartRX();

	while(DataReceived == 0)
	{
		NRF_GetStatus(&Status);
		if ((Status & RX_DR) == RX_DR)
		{
			printf("*** Data received ***\n");
			Ret = NRF_ReadRXPayload(TestBuffer,sizeof(TestBuffer),&Status);
			printf("%s\n", TestBuffer);
			DataReceived = 1;
			NRF_ClearRX_DR(&Status);
		}
		sleep(1);
	}

	NRF_StopRX();
}

int main(int argc, char *argv[])
{	
    char Status;
    int  Ret;
    
    if ((argc != 2) || ((argc == 2) && (strcmp(argv[1],"TX") && strcmp(argv[1],"RX"))))
	{
		printf("Usage : nrftest TX or RX\n");
		exit(-1);
	}

	printf("*** Test nrf lib ***\n");
    Ret = NRF_Init();
    
    printf("Initialisation done\n");
    Ret = NRF_GetStatus(&Status);
    NRF_DisplayStatus(Status);

    Ret = NRF_SetPowerMode(POWER_ON, &Status);
    Ret = NRF_FlushTX(&Status);
    Ret = NRF_FlushRX(&Status);

    if (strcmp(argv[1],"TX") == 0)
    {
		Test_Send();
    }
    else if (strcmp(argv[1],"RX") == 0)
    {
		Test_Receive();
    }

    Ret = NRF_SetPowerMode(POWER_OFF, &Status);

    return 0;
}
