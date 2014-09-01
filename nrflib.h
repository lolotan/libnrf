/**********************************************/
/* NRF24L01x functions header for nrf library */
/**********************************************/

#include "nrf24l01.h"

typedef enum
{
  DR1MBPS,
  DR2MBPS,
 #ifdef NRF24L01P
  DR250KBPS
#endif  
} DataRate;

typedef enum
{
    POWER_ON,
    POWER_OFF
} PowerMode;

typedef enum
{
    PA18DBM = 0x00,
    PA12DBM = 0x01,
    PA6DBM  = 0x02,
    PA0DBM  = 0x03
} PACtrl;

typedef enum
{
    LNAGainOFF = 0x00,
    LNAGainON  = 0x01
} LNAGain;

typedef enum
{
    P0 = 0x00,
    P1,
    P2,
    P3,
    P4,
    P5
} DataPipe;

// Following timings values has a security margin added
#define TIME_TX_PULSE	12		// Min TX pulse
#define TIME_TPD2STBY	4600	// Power down -> Standby
#define TIME_TSTBY2A	140		// Standby modes -> TX/RX mode
#define TIME_TPECE3CSN	5       // CE positive edge to CSN LOW

int NRF_Init(void);

int NRF_SendCommand(char, char *);

int NRF_ReadRegisterMB(char, char *, int, char *);
int NRF_ReadRegister(char, char *, char *);
int NRF_WriteRegisterMB(char, char *, int, char *);
int NRF_WriteRegister(char, char , char *);

int NRF_WriteTXPayload(char *, int, char *);
int NRF_ReadRXPayload(char *, int, char *);

int NRF_GetStatus(char *);

int NRF_SetModePRX(char *);
int NRF_SetModePTX(char *);
int NRF_SetPowerMode(PowerMode, char *);
int NRF_SetDataRate(DataRate, char *);
int NRF_SetRFChannel(int, char *);
int NRF_SetPAControl(PACtrl, char *);

// int NRF_SetAutoRetransmitDelay(int, char *); // p33/78
// int NRF_SetAutoRetransmitCount(int, char *);
// int NRF_GetLostPacketsCount(int *);
// int NRF_GetLostRetriesCount(int *);

// int NRF_SetAddressWidth(int, char *);
// int NRF_SetTxAddress(char *, char *);
// int NRF_SetRxAddress(DataPipe, char *, char *);
// int NRF_EnableDataPie(DataPipe);




