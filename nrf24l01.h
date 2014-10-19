/****************************************/
/* NRF24L01 definitions for nrf library */
/****************************************/

#define NRF24L01P

// NRF24L01 Commands
#define R_REGISTER			0x00	// Read register
#define W_REGISTER			0x20	// Write register
#define R_RX_PAYLOAD   		0x61  	// Read RX payload
#define W_TX_PAYLOAD   		0xA0  	// Write TX payload
#define FLUSH_TX      		0xE1  	// Flush TX register
#define FLUSH_RX      		0xE2  	// Flush RX register
#define REUSE_TX_PL   		0xE3  	// Reuse TX payload
#define ACTIVATE   			0x50  	// Activate R_RX_PL_WID, W_ACK_PAYLOAD and 
#define R_RX_PL_WID   		0x60  	// Read RX payload width
#define W_ACK_PAYLOAD  		0xA8  	// Write ACK payload
#define W_TX_PAYLOAD_NOACK	0xB0  	// Disable AUTO ACK
#define NOP           		0xFF  	// No Operation, used for reading status register

// NRF24L01 Registers
#define REGISTER_MASK       0x1F
#define CONFIG				0x00	// Configuration register
#define EN_AA				0x01	// Enable Auto Acknowledgement function
#define EN_RXADDR			0x02	// Enabled RX Addresses
#define SETUP_AW			0x03	// Setup of Address Widths
#define SETUP_RETR			0x04	// Setup of automatic retransmission
#define RF_CH				0x05	// RF Channel
#define RF_SETUP			0x06	// RF Setup Register
#define STATUS				0x07	// Status register
#define OBSERVE_TX			0x08	// Transmit observe register
#define CD					0x09	// Carrier detect
#define RX_ADDR_P0			0x0A	// Receive address data pipe 0
#define RX_ADDR_P1			0x0B	// Receive address data pipe 1
#define RX_ADDR_P2			0x0C	// Receive address data pipe 2
#define RX_ADDR_P3			0x0D	// Receive address data pipe 3
#define RX_ADDR_P4			0x0E	// Receive address data pipe 4
#define RX_ADDR_P5			0x0F	// Receive address data pipe 5
#define TX_ADDR				0x10	// Transmit address
#define	RX_PW_P0			0x11	// Number of bytes in RX payload in data pipe 0	
#define	RX_PW_P1			0x12	// Number of bytes in RX payload in data pipe 1
#define	RX_PW_P2			0x13	// Number of bytes in RX payload in data pipe 2
#define	RX_PW_P3			0x14	// Number of bytes in RX payload in data pipe 3
#define	RX_PW_P4			0x15	// Number of bytes in RX payload in data pipe 4
#define	RX_PW_P5			0x16	// Number of bytes in RX payload in data pipe 5
#define FIFO_STATUS			0x17	// FIFO Status register
#define DYNPD				0x1C	// Enable dynamic payload length
#define FEATURE				0x1D	// Feature register


// Bits for register bits
#define BYTE_MASK           0xFF
#define BIT0				0x01
#define BIT1				0x02
#define BIT2				0x04
#define BIT3				0x08
#define BIT4				0x10
#define BIT5				0x20
#define BIT6				0x40
#define BIT7				0x80

// NRF24L01 Register bits
// CONFIG
#define MASK_RX_DR			BIT6
#define MASK_TX_DS			BIT5
#define MASK_MAX_RT			BIT4
#define EN_CRC				BIT3
#define CRCO				BIT2
#define PWR_UP				BIT1
#define PRIM_RX				BIT0

// EN_AA
#define ENAA_P5				BIT5
#define ENAA_P4				BIT4
#define ENAA_P3				BIT3
#define ENAA_P2				BIT2
#define ENAA_P1				BIT1
#define ENAA_P0				BIT0

// EN_RXADDR
#define ERX_P5				BIT5
#define ERX_P4				BIT4
#define ERX_P3				BIT3
#define ERX_P2				BIT2
#define ERX_P1				BIT1
#define ERX_P0				BIT0

// RF_SETUP
#define PLL_LOCK			BIT4
#ifdef NRF24L01P
#define RF_DR_LOW           BIT5
#define RF_DR_HIGH			BIT3
#else
#define RF_DR   			BIT3
#endif
#define RF_PWR				(BIT2 | BIT1)
#define LNA_HCURR			BIT0

// STATUS
#define RX_DR				BIT6
#define TX_DS				BIT5
#define MAX_RT				BIT4
#define RX_P_NO				(BIT3 | BIT2 | BIT1)
#define TX_FULL				BIT0

// OBSERVE_TX
#define PLOS_CNT			(BIT7 | BIT6 | BIT5 | BIT4)
#define ARC_CNT				(BIT3 | BIT2 | BIT1 | BIT0)

// FIFO_STATUS
#define TX_REUSE			BIT6
#define TX_FIFO_FULL		BIT5
#define TX_EMPTY			BIT4
#define RX_FULL				BIT1
#define RX_EMPTY			BIT0	

// DYNPD
#define DPL_P5				BIT5
#define DPL_P4				BIT4
#define DPL_P3				BIT3
#define DPL_P2				BIT2
#define DPL_P1				BIT1
#define DPL_P0				BIT0

// FEATURE
#define EN_DPL				BIT2
#define EN_ACK_PAY			BIT1
#define EN_DYN_ACK			BIT0
