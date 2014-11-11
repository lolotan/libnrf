/******************************************/
/* Main test program for nrf library test */
/******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "nrflib.h"

void Test_Send(void)
{
	char Status;
	char RegVal;
	int  Ret;
	char TestBuffer[32];
	const char * TestStr = "Hello World !";

	printf("SEND TEST\n");

	memset(TestBuffer, 0x00, sizeof(TestBuffer));
	Ret = NRF_SetModePTX(&Status);
	Ret = NRF_ReadRegister(CONFIG, &RegVal, &Status);
	printf("CONFIG Value : %02X\n", RegVal);

	memcpy(TestBuffer, TestStr, strlen(TestStr));
	printf("Test buffer : %s\n", TestBuffer);

	Ret = NRF_WriteTXPayload(TestBuffer, 32, &Status);

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
	char RegVal;
	int  Ret;
	int  DataReceived = 0;
	char TestBuffer[32];

	printf("RECEIVE TEST\n");

	memset(TestBuffer, 0x00, sizeof(TestBuffer));
	Ret = NRF_SetModePRX(&Status);
	Ret = NRF_ReadRegister(CONFIG, &RegVal, &Status);
	printf("CONFIG Value : %02X\n", RegVal);
	NRF_StartRX();

	while(DataReceived == 0)
	{
		NRF_GetStatus(&Status);
		NRF_DisplayStatus(Status);
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
    char Addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    
    if ((argc != 2) || ((argc == 2) && (strcmp(argv[1],"TX") && strcmp(argv[1],"RX"))))
	{
		printf("Usage : nrftest TX or RX\n");
		exit(-1);
	}

	printf("*** Test nrf lib ***\n");
    Ret  = NRF_Init();
    Ret = NRF_SetPowerMode(POWER_ON, &Status);
    Ret += NRF_ClearRX_DR(&Status);
    Ret += NRF_ClearMAX_RT(&Status);
    Ret += NRF_ClearTX_DS(&Status);
    /*Ret += NRF_SetDataRate(DR2MBPS, &Status);
    Ret += NRF_SetRFChannel(2, &Status);
    Ret += NRF_SetPAControl(PA0DBM, &Status);
    Ret += NRF_SetAutoRetransmitDelay(15, &Status);
    Ret += NRF_SetAutoRetransmitCount(15, &Status);
    Ret += NRF_SetAddressWidth(5, &Status);
    Ret += NRF_SetTxAddress(Addr, &Status);
    Ret += NRF_SetRxAddress(P0, Addr, &Status);*/

    printf("Initialisation done - Ret : %d\n", Ret);

    Ret = NRF_GetStatus(&Status);
    NRF_DisplayStatus(Status);


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
