/*
 * NRF24L01_DRIVER.h
 *
 * Created: 1/1/2016 7:56:27 PM
 *  Author: teamflor
 */ 


#ifndef NRF24L01_DRIVER_H_
#define NRF24L01_DRIVER_H_

#include <asf.h>

//NRF COMMANDS
#define R_REGISTER 0
#define W_REGISTER 0b00100000

#define R_RX_PAYLOAD 0b01100001
#define W_TX_PAYLOAD 0b10100000
#define FLUSH_TX 0b11100001
#define FLUSH_RX 0b11100010
#define REUSE_TX_PL 0b11100011
#define NOP 0b11111111
//NRF REGISTERS
#define CONFIG 0
#define EN_AA 1
#define EN_RXADDR 2
#define SETUP_AW 3
#define SETUP_RETR 4
#define RF_CH 5
#define RF_SETUP 6
#define STATUS 7
#define OBSERVE_TX 8
#define CD 9
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17

#define CLEAR_TX_INT 0b00100000

//#define CE_OFF GPIO_DRV_ClearPinOutput(RF_CE)
//#define CE_ON GPIO_DRV_SetPinOutput(RF_CE)
#define CE_OFF port_pin_set_output_level(EXT2_PIN_GPIO_0, 0)
#define CE_ON port_pin_set_output_level(EXT2_PIN_GPIO_0, 1)

#define BUF_LENGTH 20
#define SLAVE_SELECT_PIN EXT2_PIN_SPI_SS_0
struct spi_module spi_master_instance;
struct spi_slave_inst slave;

void configure_spi_master(void);

enum status_code DSPI_DRV_MasterTransferBlocking(struct spi_module *const module,
uint8_t *tx_data, uint8_t *rx_data, uint16_t length);


void (*Delay_ms)(uint32_t);

void Init_NRF24L(void);
void Init_NRF24L_Transmitter(void);
void Init_NRF24L_Reciever(void);
void Set_NRF24L_Rx_Mode(void);
void Set_NRF24L_Tx_Mode(void);
unsigned char Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead);
unsigned char Write_Register(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char * RecievedArray, unsigned char NumOfBytesToRead);
unsigned char Write_Reg_Varified(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char NumOfBytesToRead);
unsigned char Send_Last_Payload(unsigned char NumberOfTimesToSend);
unsigned char Read_Status(void);
unsigned char Flush_Tx(void);
unsigned char Flush_Rx(void);
unsigned char SendNewPayload(unsigned char * PayloadArrayToSend, unsigned char NumberOfBytesToSend);
unsigned char ReadPayload(unsigned char * ReadArray, unsigned char NumberOfBytesToRead);
unsigned char WatchForPayload(unsigned char * ReadArray, unsigned char NumberOfBytesToRead);
void Clear_NRF_Int_Flags(void);



#endif /* NRF24L01_DRIVER_H_ */