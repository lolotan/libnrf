/***************************************/
/* NRF24L01x functions for nrf library */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "spi.h"
#include "gpio.h"
#include "timer.h"
#include "nrflib.h"

int NRF_Init(void)
{
	int ret = -1;
	GPIO_Init();
	TIMER_Init();
	ret = SPI_Init();
	return ret;
}

int NRF_GetStatus(char * RetStatus)
{
	return SPI_SendCommand(NOP, RetStatus);
}

int NRF_FlushTX(char * RetStatus)
{
	return SPI_SendCommand(FLUSH_TX, RetStatus);
}

int NRF_FlushRX(char * RetStatus)
{
	return SPI_SendCommand(FLUSH_RX, RetStatus);
}

int NRF_ReuseTX_PL(char * RetStatus)
{
	return SPI_SendCommand(REUSE_TX_PL, RetStatus);
}

int NRF_ReadRXPLWidth(int * Width, char * RetStatus)
{
	return SPI_CommandRead(R_RX_PL_WID, (char *)Width, 1, RetStatus);
}

int NRF_ReadRegisterMB(char Register, char * ReadBuffer, int Length, char * RetStatus)
{
    return SPI_CommandRead((Register | R_REGISTER), ReadBuffer, Length, RetStatus);
}

int NRF_ReadRegister(char Register, char * RegisterValue, char * RetStatus)
{
    return SPI_CommandRead((Register | R_REGISTER), RegisterValue, 1, RetStatus);
}

int NRF_WriteRegisterMB(char Register, char * WriteBuffer, int Length, char * RetStatus)
{
    return SPI_CommandWrite((Register | W_REGISTER), WriteBuffer, Length, RetStatus);
}

int NRF_WriteRegister(char Register, char RegisterValue, char * RetStatus)
{
    return SPI_CommandWrite((Register | W_REGISTER), &RegisterValue, 1, RetStatus);
}

int NRF_WriteTXPayload(char * TXPayload, int Length, char * RetStatus)
{
    return SPI_CommandWrite(W_TX_PAYLOAD, TXPayload, Length, RetStatus);
}

int NRF_WriteACKPayload(char * TXPayload, DataPipe Pipe, int Length, char * RetStatus)
{
    return SPI_CommandWrite((W_ACK_PAYLOAD | Pipe), TXPayload, Length, RetStatus);
}

int NRF_WritePayloadNOACK(char * TXPayload, int Length, char * RetStatus)
{
    return SPI_CommandWrite(W_TX_PAYLOAD_NOACK, TXPayload, Length, RetStatus);
}

int NRF_ReadRXPayload(char * RXPayload, int Length, char * RetStatus)
{
    return SPI_CommandRead(R_RX_PAYLOAD, RXPayload, Length, RetStatus);
}

int NRF_SetModePRX(char * RetStatus)
{
    char ConfigRegister = 0;
    int Ret;
    
    Ret = NRF_ReadRegister(CONFIG, &ConfigRegister, RetStatus);
    if (Ret < 0)
        return Ret;
    
    ConfigRegister |= PRIM_RX;      // Set PRX Mode with PRIM_RX = 1
    
    Ret = NRF_WriteRegister(CONFIG, ConfigRegister, RetStatus);
    
    TIMER_Wait_us(TIME_TSTBY2A);    // Wait Tstby2a time
    return Ret;
}

int NRF_SetModePTX(char * RetStatus)
{
    char ConfigRegister;
    int Ret;
    
    Ret = NRF_ReadRegister(CONFIG, &ConfigRegister, RetStatus);
    if (Ret < 0)
        return Ret;

    ConfigRegister &= ~PRIM_RX ;    // Set PTX Mode with PRIM_RX = 0
    
    Ret = NRF_WriteRegister(CONFIG, ConfigRegister, RetStatus);
    
    TIMER_Wait_us(TIME_TSTBY2A);    // Wait Tstby2a time
    return Ret;
}

int NRF_SetPowerMode(PowerMode Power, char * RetStatus)
{
    char ConfigRegister;
    int Ret;

    Ret = NRF_ReadRegister(CONFIG, &ConfigRegister, RetStatus);
    if (Ret < 0)
        return Ret;
        
    if (Power == POWER_OFF)
        ConfigRegister &= ~PWR_UP;  // PWR_UP = 0
    else
        ConfigRegister |= PWR_UP;   // PWR_UP = 1
    
    Ret = NRF_WriteRegister(CONFIG, ConfigRegister, RetStatus);
    
    TIMER_Wait_us(TIME_TPD2STBY);   // Wait Tpd2stby time
    return Ret;
}

int NRF_SetDataRate(DataRate DataRateValue, char * RetStatus)
{
    char RFSetup = 0x00;
    
    switch(DataRateValue)
    {
        case DR1MBPS:
            RFSetup &= ~RF_DR_HIGH;
            RFSetup &= ~RF_DR_LOW;
            break;
            
        case DR2MBPS:
            RFSetup |= RF_DR_HIGH;
            RFSetup &= ~RF_DR_LOW;
            break;
                    
        case DR250KBPS:
            RFSetup &= ~RF_DR_HIGH;
            RFSetup |= RF_DR_LOW;
            break;
        
        default:
            RFSetup |= RF_DR_HIGH;
            RFSetup &= ~RF_DR_LOW;
    }
    return NRF_WriteRegister(RF_SETUP, RFSetup, RetStatus);
}

int NRF_SetRFChannel(int RFChannel, char * RetStatus)
{
    int Ret;
    RFChannel &= 0x7F;
    Ret = NRF_WriteRegister(RF_CH, (char)RFChannel, RetStatus);
    return Ret;
}

int NRF_SetPAControl(PACtrl PACtrlValue, char * RetStatus)
{
    int Ret;
    char RFSetup;
    
    Ret = NRF_ReadRegister(RF_SETUP, &RFSetup, RetStatus);
    if (Ret < 0)
        return Ret;
        
    RFSetup &= 0xF9;
    RFSetup |= ((PACtrlValue << 1) & 0x06);
    Ret = NRF_WriteRegister(RF_SETUP, RFSetup, RetStatus); 
    return Ret;
}

int NRF_ClearRX_DR(char * RetStatus)
{
    int Ret;
    char StatusReg;
    Ret = NRF_ReadRegister(STATUS, &StatusReg, RetStatus);
    if (Ret < 0)
        return Ret;
    StatusReg |= RX_DR;
    Ret = NRF_WriteRegister(STATUS, StatusReg, RetStatus);
    return Ret;
}

int NRF_ClearTX_DS(char * RetStatus)
{
	int Ret;
	char StatusReg;
	Ret = NRF_ReadRegister(STATUS, &StatusReg, RetStatus);
	if (Ret < 0)
		return Ret;
	StatusReg |= TX_DS;
	Ret = NRF_WriteRegister(STATUS, StatusReg, RetStatus);
	return Ret;
}

int NRF_ClearMAX_RT(char *  RetStatus)
{
	int Ret;
	char StatusReg;
	Ret = NRF_ReadRegister(STATUS, &StatusReg, RetStatus);
	if (Ret < 0)
		return Ret;
	StatusReg |= MAX_RT;
	Ret = NRF_WriteRegister(STATUS, StatusReg, RetStatus);
	return Ret;
}

void NRF_DisplayStatus(char Status)
{
	printf("*** STATUS REGISTER ***\n");
	printf(" RX_DR   : %d\n", (Status & RX_DR)   >> 6);
	printf(" TX_DS   : %d\n", (Status & TX_DS)   >> 5);
	printf(" MAX_RT  : %d\n", (Status & MAX_RT)  >> 4);
	if (((Status & RX_P_NO) >> 1) == 7)
		printf(" RX_P_NO : EMPTY\n");
	else
		printf(" RX_P_NO : %d\n", (Status & RX_P_NO) >> 1);
	printf(" TX_FULL : %d\n", (Status & TX_FULL));
	printf("***********************\n");
}

int NRF_SetAutoRetransmitDelay(int Delay, char * RetStatus)
{
	// ARD
    return NRF_WriteRegister(SETUP_RETR, (char)((Delay & 0x0F) << 4), RetStatus);
}

int NRF_SetAutoRetransmitCount(int Count, char * RetStatus)
{
	// ARC
	return NRF_WriteRegister(SETUP_RETR, (char)(Count & 0x0F), RetStatus);
}

int NRF_GetLostPacketsCount(int * Count, char * RetStatus)
{
	// PLOS_CNT
    int Ret;
    char ObserveTxReg;
    Ret = NRF_ReadRegister(OBSERVE_TX, &ObserveTxReg, RetStatus);
    *Count = (int)((ObserveTxReg & 0xF0) >> 4);
    return Ret;
}

int NRF_GetLostRetriesCount(int * Count, char * RetStatus)
{
	// ARC_CNT
    int Ret;
    char ObserveTxReg;
	Ret = NRF_ReadRegister(OBSERVE_TX, &ObserveTxReg, RetStatus);
	*Count = (int)(ObserveTxReg & 0x0F);
	return Ret;
}

int NRF_SetAddressWidth(int Width, char * RetStatus)
{
    if ((Width < 3) || (Width > 5))
    	return NRF_ERROR;

    return NRF_WriteRegister(SETUP_AW, (Width & 0x03), RetStatus);
}

int NRF_SetTxAddress(const char * Address, char * RetStatus)
{
    return NRF_WriteRegisterMB(TX_ADDR, (char *)Address, 5, RetStatus);
}

int NRF_SetRxAddress(DataPipe DPipe, const char * Address, char * RetStatus)
{
	char RxPipeAddr;

    switch (DPipe)
    {
    case P0:
    	RxPipeAddr = RX_ADDR_P0;
    	break;
    case P1:
    	RxPipeAddr = RX_ADDR_P1;
        break;
    case P2:
    	RxPipeAddr = RX_ADDR_P2;
        break;
    case P3:
    	RxPipeAddr = RX_ADDR_P3;
        break;
    case P4:
    	RxPipeAddr = RX_ADDR_P4;
        break;
    case P5:
    	RxPipeAddr = RX_ADDR_P5;
        break;
    default:
    	RxPipeAddr = RX_ADDR_P0;
    }

    return NRF_WriteRegisterMB(RxPipeAddr, (char *)Address, 5, RetStatus);
}

int NRF_EnableDataPipe(DataPipe DPipe, char * RetStatus)
{
    int  Ret;
	char EnabledRX;

	Ret = NRF_ReadRegister(EN_RXADDR, &EnabledRX, RetStatus);
	if (Ret < 0)
		return Ret;

    switch (DPipe)
    {
    case P0:
    	EnabledRX |= ERX_P0;
    	break;
    case P1:
    	EnabledRX |= ERX_P1;
        break;
    case P2:
    	EnabledRX |= ERX_P2;
        break;
    case P3:
    	EnabledRX |= ERX_P3;
        break;
    case P4:
    	EnabledRX |= ERX_P4;
        break;
    case P5:
    	EnabledRX |= ERX_P5;
        break;
    }

    return NRF_WriteRegister(EN_RXADDR, EnabledRX, RetStatus);
}

int NRF_SetDataPipeLength(DataPipe DPipe, int Length, char * RetStatus)
{
	char RxPayloadReg;

    switch (DPipe)
	{
	case P0:
		RxPayloadReg = RX_PW_P0;
		break;
	case P1:
		RxPayloadReg = RX_PW_P1;
		break;
	case P2:
		RxPayloadReg = RX_PW_P2;
		break;
	case P3:
		RxPayloadReg = RX_PW_P3;
		break;
	case P4:
		RxPayloadReg = RX_PW_P4;
		break;
	case P5:
		RxPayloadReg = RX_PW_P5;
		break;
	}

    return NRF_WriteRegister(RxPayloadReg, (char)(Length & 0x1F), RetStatus);
}

void NRF_StartRX(void)
{
	GPIO_SetCE(GPIO_ON);
}

void NRF_StopRX(void)
{
	GPIO_SetCE(GPIO_OFF);
}

void NRF_TXPayload(void)
{
	GPIO_SetCE(GPIO_ON);
	TIMER_Wait_us(TIME_TX_PULSE);
	GPIO_SetCE(GPIO_OFF);
}
