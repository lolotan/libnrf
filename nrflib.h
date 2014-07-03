/**********************************************/
/* NRF24L01x functions header for nrf library */
/**********************************************/

#include "nrf24l01.h"
#include "spi.h"
#include "gpio.h"
#include "timer.h"

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

// Following timings values has a security margin added
#define TIME_TX_PULSE	12		// Min TX pulse
#define TIME_TPD2STBY	4600	// Power down -> Standby
#define TIME_TSTBY2A	140		// Standby modes -> TX/RX mode
#define TIME_TPECE3CSN	5		// CE positive edge to CSN LOW

int NRF_Init(void);

int NRF_SendCommand(char, char *);
int NRF_ReadRegister(char, char *, int, char *);
int NRF_WriteRegister(char, char *, int, char *);
int NRF_WriteTXPayload(char *, int, char *);
int NRF_ReadRXPayload(char *, int, char *);

int NRF_GetStatus(char *);

int NRF_SetModePRX(char *);
int NRF_SetModePTX(char *);
int NRF_SetPowerMode(PowerMode, char *);
int NRF_SetDataRate(DataRate, char *);
int NRF_SetRFChannel(int, char *);
int NRF_SetPAControl(PACtrl, char *);

