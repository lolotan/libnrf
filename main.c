/******************************************/
/* Main test program for nrf library test */
/******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "nrflib.h"

void test_send(void)
{
	char status;
	char regval;
	int  ret;
	char testbuffer[32];
	const char * teststr = "Hello World !";

	printf("SEND TEST\n");

	memset(testbuffer, 0x00, sizeof(testbuffer));
	ret = nrf_set_mode_ptx(&status);
	ret = nrf_read_register(CONFIG, &regval, &status);
	printf("CONFIG Value : %02X\n", regval);

	memcpy(testbuffer, teststr, strlen(teststr));
	printf("Test buffer : %s\n", testbuffer);

	ret = nrf_write_tx_payload(testbuffer, 32, &status);

	nrf_tx_payload();
	printf("Payload transmitted\n");

	ret = nrf_get_status(&status);
	nrf_display_status(status);

	int packetcnt;
	ret = nrf_get_lost_packets_count(&packetcnt, &status);
	printf("Lost Packets count : %d\n", packetcnt);

	ret = nrf_get_lost_retries_count(&packetcnt, &status);
	printf("Lost Packets retries : %d\n", packetcnt);
}

void test_receive(void)
{
	char status;
	char regval;
	int  ret;
	int  datarecv = 0;
	int  loopcnt = 30;
	char testbuffer[32];

	printf("RECEIVE TEST\n");

	memset(testbuffer, 0x00, sizeof(testbuffer));
	ret = nrf_set_mode_prx(&status);
	ret = nrf_read_register(CONFIG, &regval, &status);
	printf("CONFIG Value : %02X\n", regval);
	regval = 0x20;
	ret = nrf_set_datapipe_length(P0, 32, &status);
	
	nrf_start_rx();

	while ((datarecv == 0) && (loopcnt--))
	{
		nrf_get_status(&status);
		nrf_display_status(status);
		if ((status & RX_DR) == RX_DR)
		{
			printf("*** Data received ***\n");
			ret = nrf_read_rx_payload(testbuffer,sizeof(testbuffer),&status);
			printf("%s\n", testbuffer);
			datarecv = 1;
			nrf_clear_rx_dr(&status);
		}
		sleep(1);
	}

	nrf_stop_rx();
}

int main(int argc, char *argv[])
{	
    char status;
    int  ret;
    char addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
    
    if ((argc != 2) || ((argc == 2) && (strcmp(argv[1],"TX") && strcmp(argv[1],"RX"))))
	{
		printf("Usage : nrftest TX or RX\n");
		exit(-1);
	}

	printf("*** Test nrf lib ***\n");
    ret  = nrf_init();
    ret  = nrf_set_power_mode(POWER_ON, &status);
    ret += nrf_clear_rx_dr(&status);
    ret += nrf_clear_max_rt(&status);
    ret += nrf_clear_tx_ds(&status);

    printf("Initialisation done - ret : %d\n", ret);

    ret = nrf_get_status(&status);
    nrf_display_status(status);


    ret = nrf_flush_tx(&status);
    ret = nrf_flush_rx(&status);

    if (strcmp(argv[1],"TX") == 0)
    {
		test_send();
    }
    else if (strcmp(argv[1],"RX") == 0)
    {
		test_receive();
    }

    ret = nrf_set_power_mode(POWER_OFF, &status);

    return 0;
}
