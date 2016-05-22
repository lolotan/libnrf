/**********************************************/
/* NRF24L01x functions header for nrf library */
/**********************************************/

#include "nrf24l01.h"

typedef enum
{
  DR1MBPS,
  DR2MBPS,
  DR250KBPS
} datarate_t;

typedef enum
{
    POWER_ON,
    POWER_OFF
} powermode_t;

typedef enum
{
    PA18DBM = 0x00,
    PA12DBM = 0x01,
    PA6DBM  = 0x02,
    PA0DBM  = 0x03
} pactrl_t;

typedef enum
{
    LNAGainOFF = 0x00,
    LNAGainON  = 0x01
} lnagain_t;

typedef enum
{
    P0 = 0x00,
    P1,
    P2,
    P3,
    P4,
    P5
} datapipe_t;

// Following timings values has a safety margin added
#define TIME_TX_PULSE	12		// Min TX pulse
#define TIME_TPD2STBY	4600	// Power down -> Standby
#define TIME_TSTBY2A	140		// Standby modes -> TX/RX mode
#define TIME_TPECE3CSN	5       // CE positive edge to CSN LOW

#define NRF_OK			0
#define NRF_ERROR		-1

int nrf_init(void);

int nrf_flush_tx(char *);
int nrf_flush_rx(char *);
int nrf_reuse_tx_pl(char *);
int nrf_read_rx_pl_width(int *, char *);

int nrf_read_register_mb(char, char *, int, char *);
int nrf_read_register(char, char *, char *);
int nrf_write_register_mb(char, char *, int, char *);
int nrf_write_register(char, char, char *);

int nrf_write_tx_payload(char *, int, char *);
int nrf_write_ack_payload(char *, datapipe_t, int, char *);
int nrf_write_tx_payload_noack(char *, int, char *);
int nrf_read_rx_payload(char *, int, char *);

int nrf_get_status(char *);
int nrf_clear_rx_dr(char *);
int nrf_clear_tx_ds(char *);
int nrf_clear_max_rt(char *);
void nrf_display_status(char);

int nrf_set_mode_prx(char *);
int nrf_set_mode_ptx(char *);
int nrf_set_power_mode(powermode_t, char *);
int nrf_set_data_rate(datarate_t, char *);
int nrf_set_rf_channel(int, char *);
int nrf_set_pa_control(pactrl_t, char *);

int nrf_set_auto_retransmit_delay(int, char *); // p33/78
int nrf_set_auto_retransmit_count(int, char *);
int nrf_get_lost_packets_count(int *, char *);
int nrf_get_lost_retries_count(int *, char *);

int nrf_set_address_width(int, char *);
int nrf_set_tx_address(const char *, char *);
int nrf_set_rx_address(datapipe_t, const char *, char *);
int nrf_enable_datapipe(datapipe_t, char *);
int nrf_set_datapipe_length(datapipe_t, int, char *);

void nrf_start_rx(void);
void nrf_stop_rx(void);
void nrf_tx_payload(void);


