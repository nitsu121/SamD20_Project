/*
 * NRF24L01_DRIVER.c
 *
 * Created: 1/1/2016 7:57:36 PM
 *  Author: teamflor
 */ 



#include "NRF24L01_DRIVER.h"


enum status_code DSPI_DRV_MasterTransferBlocking(struct spi_module *const module,
uint8_t *tx_data, uint8_t *rx_data, uint16_t length)
{
	enum status_code ReturnFlag;
	spi_select_slave(&spi_master_instance, &slave, true);
	//enum status_code spi_transceive_buffer_wait(struct spi_module *const module, uint8_t *tx_data, uint8_t *rx_data, uint16_t length);
	ReturnFlag = spi_transceive_buffer_wait(module, tx_data, rx_data, length);
	spi_select_slave(&spi_master_instance, &slave, false);
	return (ReturnFlag);
}


void configure_spi_master(void)
{
	struct spi_config config_spi_master;
	struct spi_slave_inst_config slave_dev_config;
	/* Configure and initialize software device instance of peripheral slave */
	spi_slave_inst_get_config_defaults(&slave_dev_config);
	slave_dev_config.ss_pin = SLAVE_SELECT_PIN;
	spi_attach_slave(&slave, &slave_dev_config);
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_master);
	//config_spi_master.mux_setting = EXT1_SPI_SERCOM_MUX_SETTING;
	config_spi_master.mux_setting = EXT2_SPI_SERCOM_MUX_SETTING;
	/* Configure pad 0 for data in */
	//config_spi_master.pinmux_pad0 = EXT1_SPI_SERCOM_PINMUX_PAD0;
	config_spi_master.pinmux_pad0 = EXT2_SPI_SERCOM_PINMUX_PAD0;
	/* Configure pad 1 as unused */
	//config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	config_spi_master.pinmux_pad1 = PINMUX_UNUSED;
	/* Configure pad 2 for data out */
	//config_spi_master.pinmux_pad2 = EXT1_SPI_SERCOM_PINMUX_PAD2;
	config_spi_master.pinmux_pad2 = EXT2_SPI_SERCOM_PINMUX_PAD2;
	/* Configure pad 3 for SCK */
	//config_spi_master.pinmux_pad3 = EXT1_SPI_SERCOM_PINMUX_PAD3;
	config_spi_master.pinmux_pad3 = EXT2_SPI_SERCOM_PINMUX_PAD3;
	//spi_init(&spi_master_instance, EXT1_SPI_MODULE, &config_spi_master);
	spi_init(&spi_master_instance, EXT2_SPI_MODULE, &config_spi_master);
	spi_enable(&spi_master_instance);
}


void Clear_NRF_Int_Flags(void)
{
	unsigned char SendArray[2] = {0, 0};
	unsigned char ReadArray[2];
	unsigned char TempStatus = 0;
	TempStatus = Read_Status();
	SendArray[0] = TempStatus & 0xF0;
	Write_Register(7, SendArray, ReadArray, 1); //
}


void Set_NRF24L_Rx_Mode()
{
	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	unsigned char SendArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	Flush_Tx();
	Flush_Rx();

	SendArray[0] = 0x0F;
	Write_Reg_Varified(0x00, SendArray, 1);

	CE_ON;
}

void Set_NRF24L_Tx_Mode()
{
	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	unsigned char SendArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	Flush_Tx();
	Flush_Rx();

	SendArray[0] = 0x0E;
	Write_Reg_Varified(0x00, SendArray, 1);
}



void Init_NRF24L(void)
{
	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	unsigned char SendArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char ReadArray[10];


	SendArray[0] = 0x26; //TX not-power-up 2_byte_CRC EnableCRC
	Write_Reg_Varified(6, SendArray, 1);

	SendArray[0] = 0x05;
	Write_Reg_Varified(0x11, SendArray, 1);

	SendArray[0] = 0x00;
	Write_Reg_Varified(0x05, SendArray, 1);

	SendArray[0] = 0x11;
	SendArray[1] = 0x22;
	SendArray[2] = 0x33;
	SendArray[3] = 0x44;
	SendArray[4] = 0x55;
	Write_Register(0x0A, SendArray, ReadArray, 5);

	SendArray[0] = 0x11;
	SendArray[1] = 0x22;
	SendArray[2] = 0x33;
	SendArray[3] = 0x44;
	SendArray[4] = 0x55;
	Write_Register(0x10, SendArray, ReadArray, 5);

	SendArray[0] = 0x01;
	Write_Reg_Varified(0x02, SendArray, 1);

	Clear_NRF_Int_Flags();

	SendArray[0] = 0x01;
	Write_Reg_Varified(0x01, SendArray, 1);

	SendArray[0] = 0x2F;
	Write_Reg_Varified(0x04, SendArray, 1);

	Flush_Tx();
	Flush_Rx();

	SendArray[0] = 0x0E;
	Write_Reg_Varified(0x00, SendArray, 1);
}


void Init_NRF24L_Reciever()
{

	//unsigned char TempValue = 0;

	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	//Write_Register(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char * RecievedArray, unsigned char NumOfBytesToRead)
	unsigned char SendArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char ReadArray[10];


	SendArray[0] = 0x26; //TX not-power-up 2_byte_CRC EnableCRC
	//
	//unsigned char Write_Reg_Varified(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char NumOfBytesToRead)
	Write_Reg_Varified(6, SendArray, 1);

	SendArray[0] = 0x05;
	Write_Reg_Varified(0x11, SendArray, 1);

	SendArray[0] = 0x00;
	Write_Reg_Varified(0x05, SendArray, 1);

	SendArray[0] = 0x11;
	SendArray[1] = 0x22;
	SendArray[2] = 0x33;
	SendArray[3] = 0x44;
	SendArray[4] = 0x55;
	Write_Register(0x0A, SendArray, ReadArray, 5);
	//Write_Reg_Varified(0x0A, SendArray, 5);

	SendArray[0] = 0x11;
	SendArray[1] = 0x22;
	SendArray[2] = 0x33;
	SendArray[3] = 0x44;
	SendArray[4] = 0x55;
	Write_Register(0x10, SendArray, ReadArray, 5);
	//Write_Reg_Varified(0x10, SendArray, 5);

	SendArray[0] = 0x01;
	Write_Reg_Varified(0x02, SendArray, 1);

	Clear_NRF_Int_Flags();

	SendArray[0] = 0x01;
	Write_Reg_Varified(0x01, SendArray, 1);

	SendArray[0] = 0x2F;
	Write_Reg_Varified(0x04, SendArray, 1);

	Flush_Tx();
	Flush_Rx();

	SendArray[0] = 0x0F;
	Write_Reg_Varified(0x00, SendArray, 1);
}

#if 0
void Init_NRF24L_Reciever(void)
{

	unsigned char TempValue = 0;

	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	//Write_Register(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char * RecievedArray, unsigned char NumOfBytesToRead)
	unsigned char SendArray[2] = {0, 0};
	unsigned char ReadArray[2];

	SendArray[0] = 0x0D;
	//
	//Write_Reg_Varified(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char NumOfBytesToRead);
	Write_Reg_Varified(0, SendArray, 1);
	//

	SendArray[0] = 0x00;
	Write_Reg_Varified(1, SendArray, 1);

	SendArray[0] = 0x01;
	Write_Reg_Varified(2, SendArray, 1);

	SendArray[0] = 0x03;
	Write_Reg_Varified(3, SendArray, 1);

	SendArray[0] = 0x02;
	Write_Reg_Varified(5, SendArray, 1);

	SendArray[0] = 0x07;
	Write_Reg_Varified(6, SendArray, 1);

	SendArray[0] = 0x05;
	Write_Reg_Varified(0x11, SendArray, 1);

	SendArray[0] = 0x7F;
	Write_Reg_Varified(0, SendArray, 1);

	SendArray[0] = 0xE7;
	SendArray[1] = 0xE7;
	SendArray[2] = 0xE7;
	SendArray[3] = 0xE7;
	SendArray[4] = 0xE7;
	Write_Reg_Varified(0x0A, SendArray, 5);
}
#endif


void Init_NRF24L_Transmitter(void)
{

	unsigned char TempValue = 0;

	CE_OFF;
	//Delay_ms(100);
	(*Delay_ms)(100);
	//Write_Register(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char * RecievedArray, unsigned char NumOfBytesToRead)
	unsigned char SendArray[2] = {0, 0};
	unsigned char ReadArray[2];
	SendArray[0] = 0x52;
	while(!(0x52 == TempValue))
	{
		Write_Register(0, SendArray, ReadArray, 1); // power up rf mod.
		//Delay_ms(100);
		(*Delay_ms)(100);
		//Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
		Read_Register(0, &TempValue, 1);
		//Delay_ms(100);
		(*Delay_ms)(100);
	}


	TempValue = 10;
	SendArray[0] = 0x00;
	while(!(0x00 == TempValue))
	{
		Write_Register(1, SendArray, ReadArray, 1); // disable auto ack.
		//Delay_ms(100);
		(*Delay_ms)(100);
		//Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
		Read_Register(1, &TempValue, 1);
		//Delay_ms(100);
		(*Delay_ms)(100);
	}

	TempValue = 0;
	SendArray[0] = 0x01;
	while(!(0x01 == TempValue))
	{
		Write_Register(2, SendArray, ReadArray, 1); // enable data pipe 0.
		//Delay_ms(100);
		(*Delay_ms)(100);
		//Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
		Read_Register(2, &TempValue, 1);
		//Delay_ms(100);
		(*Delay_ms)(100);
	}

	TempValue = 0;
	SendArray[0] = 0x01;
	while(!(0x01 == TempValue))
	{
		Write_Register(3, SendArray, ReadArray, 1); // use 5 bytes address field width.
		//Delay_ms(100);
		(*Delay_ms)(100);
		//Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
		Read_Register(3, &TempValue, 1);
		//Delay_ms(100);
		(*Delay_ms)(100);
	}


	//SendArray[0] = 0x01;
	//Write_Register(4, SendArray, ReadArray, 1); // leave as default.

	//SendArray[0] = 0x01;
	//Write_Register(5, SendArray, ReadArray, 1); // leave as default this changes the address.

	//SendArray[0] = 0x01;
	//Write_Register(6, SendArray, ReadArray, 1); // leave as default.

	//SendArray[0] = 0x01;
	//Write_Register(7, SendArray, ReadArray, 1); // status register..

	TempValue = 0;
	SendArray[0] = 0x05;
	while(!(0x05 == TempValue))
	{
		Write_Register(0x11, SendArray, ReadArray, 1); // data payload widths for pipe0 5 bytes.
		//Delay_ms(100);
		(*Delay_ms)(100);
		//Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
		Read_Register(0x11, &TempValue, 1);
		//Delay_ms(100);
		(*Delay_ms)(100);
	}
}


unsigned char Write_Reg_Varified(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char NumOfBytesToRead)
{
	unsigned char ReturnFlag = 0;
	unsigned char TempValue = 0;
	unsigned char ReadArray[10];

	while(!(SendArray[0] == TempValue))
	{
		Write_Register(RegisterNumber, SendArray, ReadArray, NumOfBytesToRead);
		//Delay_ms(10);
		(*Delay_ms)(100);
		Read_Register(RegisterNumber, &TempValue, NumOfBytesToRead);
		//Delay_ms(10);
		(*Delay_ms)(100);
	}

	return(ReturnFlag);
}

unsigned char Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
{
	unsigned char ReturnFlag = 0;
	unsigned char TempSendArray[50];
	unsigned char TempReadArray[50];
	unsigned char Counter = 0;
	TempSendArray[0] = RegisterNumber;

	//DSPI_DRV_MasterTransferBlocking(FSL_DSPICOM1, NULL, &Send[0], Read, 5, 10000);
	//DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	ReturnFlag = TempReadArray[0];

	while(NumOfBytesToRead > Counter)
	{
		RecieveArray[Counter] = TempReadArray[Counter + 1];
		Counter++;
	}

	return (ReturnFlag);
}

unsigned char Write_Register(unsigned char RegisterNumber, unsigned char * SendArray, unsigned char * RecievedArray, unsigned char NumOfBytesToRead)
{
	unsigned char ReturnFlag = 0;
	unsigned char TempSendArray[50];
	unsigned char TempReadArray[50];
	unsigned char Counter = 0;
	TempSendArray[0] = RegisterNumber + 0x20;

	while(NumOfBytesToRead > Counter)
	{
		TempSendArray[Counter + 1] = SendArray[Counter];
		Counter++;
	}
	
	//enum status_code DSPI_DRV_MasterTransferBlocking(struct spi_module *const module,
	//												 uint8_t *tx_data, uint8_t *rx_data, uint16_t length)
	//DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	//DSPI_DRV_MasterTransferBlocking(FSL_DSPICOM1, NULL, &Send[0], Read, 5, 10000);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	ReturnFlag = TempReadArray[0];

	Counter = 0;

	while(NumOfBytesToRead > Counter)
	{
		RecievedArray[Counter] = TempReadArray[Counter + 1];
		Counter++;
	}

	return (ReturnFlag);
}

unsigned char Send_Last_Payload(unsigned char NumberOfTimesToSend)
{
	unsigned char ReturnFlag = 0;

	return (ReturnFlag);
}

unsigned char Read_Status()
{
	unsigned char ReturnFlag = 0;
	unsigned char ReadArray[2] = {0, 0};
	//unsigned char Read_Register(unsigned char RegisterNumber, unsigned char * RecieveArray, unsigned char NumOfBytesToRead)
	Read_Register(0x07, ReadArray, 1);
	ReturnFlag = ReadArray[0];
	return (ReturnFlag);
}


unsigned char WatchForPayload(unsigned char * ReadArray, unsigned char NumberOfBytesToRead)
{
	unsigned char ReturnFlag = 0;
	CE_ON;
	//Delay_ms(10);
	//CE_OFF;

	return (ReturnFlag);
}

unsigned char ReadPayload(unsigned char * ReadArray, unsigned char NumberOfBytesToRead)
{
	unsigned char ReturnFlag = 0;
	unsigned char TempSendArray[100];
	unsigned char TempReadArray[100];
	unsigned char Count = 0;
	TempSendArray[0] = R_RX_PAYLOAD;
	//(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	//DSPI_DRV_MasterTransferBlocking(FSL_DSPICOM1, NULL, TempSendArray, TempReadArray, NumberOfBytesToRead + 1, 10000);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumberOfBytesToRead + 1);
	while(NumberOfBytesToRead > Count)
	{

		ReadArray[Count] = TempReadArray[Count + 1];
		Count++;
	}

	Flush_Rx();

	return (ReturnFlag);
}

unsigned char SendNewPayload(unsigned char * PayloadArrayToSend, unsigned char NumberOfBytesToSend)
{
	unsigned char ReturnFlag = 0;
	unsigned char TempSendArray[50];
	unsigned char TempReadArray[50];
	unsigned char SendArray[10];
	unsigned char ReadArray[10];
	unsigned char Counter = 0;

	SendArray[0] = 0x0E; //TX not-power-up 2_byte_CRC EnableCRC
	Write_Register(0, SendArray, ReadArray, 1); // power up rf mod.
	Flush_Tx();
	Flush_Rx();
	TempSendArray[0] = W_TX_PAYLOAD;

	while(NumberOfBytesToSend > Counter)
	{
		TempSendArray[Counter + 1] = PayloadArrayToSend[Counter];
		Counter++;
	}
	
	//DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumberOfBytesToSend + 1);
	ReturnFlag = TempReadArray[0];

	CE_ON;
	//Delay_ms(1);
	(*Delay_ms)(1);
	CE_OFF;

	return (ReturnFlag);
}

unsigned char Flush_Tx()
{
	unsigned char ReturnFlag = 0;
	unsigned char ReadArray[2] = {0, 0};
	unsigned char SendArray[2] = {FLUSH_TX, 0};
	//DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, SendArray, ReadArray, 1);
	ReturnFlag = ReadArray[0];
	return (ReturnFlag);
}

unsigned char Flush_Rx()
{
	unsigned char ReturnFlag = 0;
	unsigned char ReadArray[2] = {0, 0};
	unsigned char SendArray[2] = {FLUSH_RX, 0};
	//DSPI_DRV_MasterTransferBlocking(&spi_master_instance, TempSendArray, TempReadArray, NumOfBytesToRead + 1);
	DSPI_DRV_MasterTransferBlocking(&spi_master_instance, SendArray, ReadArray, 1);
	ReturnFlag = ReadArray[0];
	return (ReturnFlag);
}

