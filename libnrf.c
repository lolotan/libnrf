/***************************************/
/* NRF24L01x functions for nrf library */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi.h"
#include "gpio.h"
#include "timer.h"
#include "libnrf.h"

int nrf_init(void)
{
	// Only spi_init has a return value
	gpio_init();
	timer_init();	
	return spi_init();
}

int nrf_get_status(char * retstatus)
{
	return spi_send_command(NOP, retstatus);
}

int nrf_flush_tx(char * retstatus)
{
	return spi_send_command(FLUSH_TX, retstatus);
}

int nrf_flush_rx(char * retstatus)
{
	return spi_send_command(FLUSH_RX, retstatus);
}

int nrf_reuse_tx_pl(char * retstatus)
{
	return spi_send_command(REUSE_TX_PL, retstatus);
}

int nrf_read_rx_pl_width(int * width, char * retstatus)
{
	return spi_command_read(R_RX_PL_WID, (char *)width, 1, retstatus);
}

int nrf_read_register_mb(char reg, char * readbuffer, int length, char * retstatus)
{
    return spi_command_read((reg | R_REGISTER), readbuffer, length, retstatus);
}

int nrf_read_register(char reg, char * regval, char * retstatus)
{
    return spi_command_read((reg | R_REGISTER), regval, 1, retstatus);
}

int nrf_write_register_mb(char reg, char * writebuffer, int length, char * retstatus)
{
    return spi_command_write((reg | W_REGISTER), writebuffer, length, retstatus);
}

int nrf_write_register(char reg, char regval, char * retstatus)
{
    return spi_command_write((reg | W_REGISTER), &regval, 1, retstatus);
}

int nrf_write_tx_payload(char * txpayload, int length, char * retstatus)
{
    return spi_command_write(W_TX_PAYLOAD, txpayload, length, retstatus);
}

int nrf_write_ack_payload(char * txpayload, datapipe_t Pipe, int length, char * retstatus)
{
    return spi_command_write((W_ACK_PAYLOAD | Pipe), txpayload, length, retstatus);
}

int nrf_write_tx_payload_noack(char * txpayload, int length, char * retstatus)
{
    return spi_command_write(W_TX_PAYLOAD_NOACK, txpayload, length, retstatus);
}

int nrf_read_rx_payload(char * rxpayload, int length, char * retstatus)
{
    return spi_command_read(R_RX_PAYLOAD, rxpayload, length, retstatus);
}

int nrf_set_mode_prx(char * retstatus)
{
    char confreg = 0;
    int ret;
    
    ret = nrf_read_register(CONFIG, &confreg, retstatus);
    if (ret < 0)
        return ret;
    
    confreg |= PRIM_RX;      // Set PRX Mode with PRIM_RX = 1
    
    ret = nrf_write_register(CONFIG, confreg, retstatus);
    
    timer_wait_us(TIME_TSTBY2A);    // Wait Tstby2a time
    return ret;
}

int nrf_set_mode_ptx(char * retstatus)
{
    char confreg;
    int ret;
    
    ret = nrf_read_register(CONFIG, &confreg, retstatus);
    if (ret < 0)
        return ret;

    confreg &= ~PRIM_RX ;    // Set PTX Mode with PRIM_RX = 0
    
    ret = nrf_write_register(CONFIG, confreg, retstatus);
    
    timer_wait_us(TIME_TSTBY2A);    // Wait Tstby2a time
    return ret;
}

int nrf_set_power_mode(powermode_t Power, char * retstatus)
{
    char confreg;
    int ret;

    ret = nrf_read_register(CONFIG, &confreg, retstatus);
    if (ret < 0)
        return ret;
        
    if (Power == POWER_OFF)
        confreg &= ~PWR_UP;  // PWR_UP = 0
    else
        confreg |= PWR_UP;   // PWR_UP = 1
    
    ret = nrf_write_register(CONFIG, confreg, retstatus);
    
    timer_wait_us(TIME_TPD2STBY);   // Wait Tpd2stby time
    return ret;
}

int nrf_set_data_rate(datarate_t datarateval, char * retstatus)
{
    int ret;
    char rfsetup;
    
    ret = nrf_read_register(RF_SETUP, &rfsetup, retstatus);
    if (ret < 0)
        return ret;
    
    switch(datarateval)
    {
        case DR1MBPS:
            rfsetup &= ~RF_DR_HIGH;
            rfsetup &= ~RF_DR_LOW;
            break;
            
        case DR2MBPS:
            rfsetup |= RF_DR_HIGH;
            rfsetup &= ~RF_DR_LOW;
            break;
                    
        case DR250KBPS:
            rfsetup &= ~RF_DR_HIGH;
            rfsetup |= RF_DR_LOW;
            break;
        
        default:
            rfsetup |= RF_DR_HIGH;
            rfsetup &= ~RF_DR_LOW;
    }
    return nrf_write_register(RF_SETUP, rfsetup, retstatus);
}

int nrf_set_rf_channel(int rfchannel, char * retstatus)
{
    rfchannel &= 0x7F;
    return nrf_write_register(RF_CH, (char)rfchannel, retstatus);
}

int nrf_get_rf_channel(int * rfchannel, char * retstatus)
{
    int ret;
    char reg_val;
    ret = nrf_read_register(RF_CH, &reg_val, retstatus);
    *rfchannel = reg_val;
    return ret;
}

int nrf_set_pa_control(pactrl_t pactrlval, char * retstatus)
{
    int ret;
    char rfsetup;
    
    ret = nrf_read_register(RF_SETUP, &rfsetup, retstatus);
    if (ret < 0)
        return ret;
        
    rfsetup &= 0xF9;
    rfsetup |= ((pactrlval << 1) & 0x06);
    ret = nrf_write_register(RF_SETUP, rfsetup, retstatus); 
    return ret;
}

int nrf_get_pa_control(pactrl_t * pactrlval, char * retstatus)
{
    int ret;
    char rfsetup;
    
    ret = nrf_read_register(RF_SETUP, &rfsetup, retstatus);
    if (ret < 0)
        return ret;
    
    *pactrlval = (pactrl_t)(rfsetup &= RF_PWR);
    return ret;
}

int nrf_clear_rx_dr(char * retstatus)
{
    int ret;
    char statusreg;
    ret = nrf_read_register(STATUS, &statusreg, retstatus);
    if (ret < 0)
        return ret;
    statusreg |= RX_DR;
    ret = nrf_write_register(STATUS, statusreg, retstatus);
    return ret;
}

int nrf_clear_tx_ds(char * retstatus)
{
	int ret;
	char statusreg;
	ret = nrf_read_register(STATUS, &statusreg, retstatus);
	if (ret < 0)
		return ret;
	statusreg |= TX_DS;
	ret = nrf_write_register(STATUS, statusreg, retstatus);
	return ret;
}

int nrf_clear_max_rt(char *  retstatus)
{
	int ret;
	char statusreg;
	ret = nrf_read_register(STATUS, &statusreg, retstatus);
	if (ret < 0)
		return ret;
	statusreg |= MAX_RT;
	ret = nrf_write_register(STATUS, statusreg, retstatus);
	return ret;
}

void nrf_display_status(char Status)
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

int nrf_set_auto_retransmit_delay(int delay, char * retstatus)
{
    // ARD
    int  ret;
    char reg;
    ret = nrf_read_register(SETUP_RETR, &reg, retstatus);
    if (ret < 0)
        return ret;
    reg &=0x0F;
    reg |= (char)((delay & 0x0F) << 4);
    return nrf_write_register(SETUP_RETR, reg, retstatus);
}

int nrf_set_auto_retransmit_count(int count, char * retstatus)
{
    // ARC
    int  ret;
    char reg;
    ret = nrf_read_register(SETUP_RETR, &reg, retstatus);
    if (ret < 0)
        return ret;
    reg &=0xF0;
    reg |= (char)(count & 0x0F);
    return nrf_write_register(SETUP_RETR, reg, retstatus);
}

int nrf_get_lost_packets_count(int * count, char * retstatus)
{
	// PLOS_CNT
    int ret;
    char obstxreg;
    ret = nrf_read_register(OBSERVE_TX, &obstxreg, retstatus);
    *count = (int)((obstxreg & 0xF0) >> 4);
    return ret;
}

int nrf_get_lost_retries_count(int * count, char * retstatus)
{
    // ARC_CNT
    int ret;
    char obstxreg;
    ret = nrf_read_register(OBSERVE_TX, &obstxreg, retstatus);
    *count = (int)(obstxreg & 0x0F);
    return ret;
}

int nrf_set_address_width(int width, char * retstatus)
{
    if ((width < 3) || (width > 5))
    	return NRF_ERROR;

    return nrf_write_register(SETUP_AW, (width & 0x03), retstatus);
}

int nrf_set_tx_address(const char * address, char * retstatus)
{
    return nrf_write_register_mb(TX_ADDR, (char *)address, 5, retstatus);
}

int nrf_set_rx_address(datapipe_t datapipe, const char * address, char * retstatus)
{
    char rxpipeaddr;

    switch (datapipe)
    {
    case P0:
    	rxpipeaddr = RX_ADDR_P0;
    	break;
    case P1:
    	rxpipeaddr = RX_ADDR_P1;
        break;
    case P2:
    	rxpipeaddr = RX_ADDR_P2;
        break;
    case P3:
    	rxpipeaddr = RX_ADDR_P3;
        break;
    case P4:
    	rxpipeaddr = RX_ADDR_P4;
        break;
    case P5:
    	rxpipeaddr = RX_ADDR_P5;
        break;
    default:
    	rxpipeaddr = RX_ADDR_P0;
    }

    return nrf_write_register_mb(rxpipeaddr, (char *)address, 5, retstatus);
}

int nrf_enable_datapipe(datapipe_t datapipe, char * retstatus)
{
    int  ret;
	char enabledrx;

	ret = nrf_read_register(EN_RXADDR, &enabledrx, retstatus);
	if (ret < 0)
		return ret;

    switch (datapipe)
    {
    case P0:
    	enabledrx |= ERX_P0;
    	break;
    case P1:
    	enabledrx |= ERX_P1;
        break;
    case P2:
    	enabledrx |= ERX_P2;
        break;
    case P3:
    	enabledrx |= ERX_P3;
        break;
    case P4:
    	enabledrx |= ERX_P4;
        break;
    case P5:
    	enabledrx |= ERX_P5;
        break;
    }

    return nrf_write_register(EN_RXADDR, enabledrx, retstatus);
}

int nrf_set_datapipe_length(datapipe_t datapipe, int length, char * retstatus)
{
	char rxplreg;

    switch (datapipe)
	{
	case P0:
		rxplreg = RX_PW_P0;
		break;
	case P1:
		rxplreg = RX_PW_P1;
		break;
	case P2:
		rxplreg = RX_PW_P2;
		break;
	case P3:
		rxplreg = RX_PW_P3;
		break;
	case P4:
		rxplreg = RX_PW_P4;
		break;
	case P5:
		rxplreg = RX_PW_P5;
		break;
	}

    return nrf_write_register(rxplreg, (char)length, retstatus);
}

int nrf_enable_dynamic_payload(datapipe_t datapipe, char * retstatus)
{    
    int  ret;
	char feature;
    char dplreg;
    
    // First, enable dynamic payload in FEATURE register
	ret = nrf_read_register(FEATURE, &feature, retstatus);
	if (ret < 0)
		return ret;
        
    if ((feature && EN_DPL) != EN_DPL)
    {
        feature |= EN_DPL;
        ret = nrf_write_register(FEATURE, feature, retstatus);
        if (ret < 0)
            return ret;
    }
    
    // Then, activating the requested datapipe dpl
    ret = nrf_read_register(DYNPD, &dplreg, retstatus);
	if (ret < 0)
		return ret;
        
    switch (datapipe)
    {
     	case P0:
		dplreg |= DPL_P0;
		break;
	case P1:
		dplreg |= DPL_P1;
		break;
	case P2:
		dplreg |= DPL_P2;
		break;
	case P3:
		dplreg |= DPL_P3;
		break;
	case P4:
		dplreg |= DPL_P4;
		break;
	case P5:
		dplreg |= DPL_P5;
		break;  
    }
    return ret = nrf_write_register(DYNPD, dplreg, retstatus);
}

int nrf_get_payload_length(int * length, char * retstatus)
{
    char reg;
    int ret;
    ret = nrf_read_register(R_RX_PL_WID, &reg, retstatus);
    *length = reg & 0x3F;
    return ret;
}

void nrf_start_rx(void)
{
	gpio_set_ce(GPIO_ON);
}

void nrf_stop_rx(void)
{
	gpio_set_ce(GPIO_OFF);
}

void nrf_tx_payload(void)
{
	gpio_set_ce(GPIO_ON);
	timer_wait_us(TIME_TX_PULSE);
	gpio_set_ce(GPIO_OFF);
}
