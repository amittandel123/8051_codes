#include <reg51.h>
#include "EEPROM.H"
#include "I2C.h"
#include "LCD.h"

typedef union Address
	{
		unsigned int VALUE;
		unsigned char BYTE[2];
	}MEM_ADDRESS;

#define send_Word_Address(x) 	send_Data(x)
#define send_Device_Address(x) 	send_Data(x)
#define MEM_DEV_ADDR 			0xA0

void Write_Add_I2C(unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	I2C_Start();	
	
	send_Device_Address(MEM_DEV_ADDR); // write mode
	device_ACK();

	send_Word_Address(byte1_Word_Address); // first byte of word address
	device_ACK();

	send_Word_Address(byte2_Word_Address); // second byte of word address
	device_ACK();
}

unsigned char Read_I2C(void)
{
	unsigned char Readbyte;
	I2C_Start();	
	send_Device_Address(MEM_DEV_ADDR+1); // read mode 
	device_ACK();
	Readbyte = Read_Data();
	Master_NO_ACK();
	I2C_Stop();	
	return(Readbyte);
}

void I2C_Write(unsigned char *mData,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	Write_Add_I2C(byte1_Word_Address, byte2_Word_Address);
	send_Data(*mData);			
	device_ACK();
	I2C_Stop();	
	DelayMs(10);
	Display_Char_LCD(*mData);
}

unsigned char I2C_Read(unsigned char Add_MSB,unsigned char Add_LSB)
{
	Write_Add_I2C(Add_MSB,Add_LSB);
	return(Read_I2C());
}
void DelayMs(unsigned int TimeCnt)
{
	unsigned int i, j; 
	for(i=0;i<TimeCnt;i++)
	{
		for(j=0;j<50;j++);
	} 
}
