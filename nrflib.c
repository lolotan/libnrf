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

int NRF_SendCommand(char Command, char * RetStatus)
{
    return SPI_SendCommand(Command, RetStatus);
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
    int Ret;
    char RFSetup;
    
    Ret = NRF_ReadRegister(RF_SETUP, &RFSetup, RetStatus);
    if (Ret < 0)
        return Ret;
    
    switch(DataRateValue)
    {
        #ifdef NRF24L01P
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
        #else
        case DR1MBPS:
            RFSetup &= ~RF_DR;
            break;
            
        case DR2MBPS:
            RFSetup |= RF_DR;
            break;
        
        default:
            RFSetup |= RF_DR;
        #endif
    }
    Ret = NRF_WriteRegister(RF_SETUP, RFSetup, RetStatus);
    return Ret;
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
    RFSetup |= ((PACtrlValue << 1) & 0xF9);
    Ret = NRF_WriteRegister(RF_SETUP, RFSetup, RetStatus); 
    return Ret;
}

int NRF_ClearRX_DR(char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_ClearTX_DS(char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_ClearMAX_RT(char *  RetStatus)
{
    int Ret;
    return Ret;
}

void NRF_DisplayStatus(char Status)
{
    int Ret;
    return Ret;
}

int NRF_SetAutoRetransmitDelay(int Delay, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_SetAutoRetransmitCount(int Count, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_GetLostPacketsCount(int * Count, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_GetLostRetriesCount(int * Count, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_SetAddressWidth(int Width, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_SetTxAddress(char * Address, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_SetRxAddress(DataPipe DPipe, char * Address, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_EnableDataPipe(DataPipe DPipe, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_SetDataPipeLength(DataPipe DPipe, int Length, char * RetStatus)
{
    int Ret;
    return Ret;
}

int NRF_StartRX(void)
{
}

int RRF_StopRX(void)
{
}

int NRF_TXPayload(void)
{
}
