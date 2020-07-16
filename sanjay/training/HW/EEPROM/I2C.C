#include <stdio.h>
#include <reg51.h>
#include <intrins.h>

bit HIGH = 1;
bit LOW = 0;
//const long XTAL =11059200;

sbit SDA = P1^0;
sbit SCL = P1^1;

void I2C_initialise(void)
{
	SCL = HIGH;
	SDA = HIGH;
}

void I2C_Start(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
	SDA = LOW;
}

void I2C_Stop(void)
{
	SCL = LOW;
	SDA = LOW;
	SCL = HIGH;
	SDA = HIGH;
}
unsigned char Read_Data(void)
{
	unsigned int i;
	unsigned char readbyte;
	for(i=0;i<8;i++)
	{
		readbyte = readbyte << 1;

		SCL = LOW;
		SCL = HIGH;
		if(SDA == 1)
			readbyte = readbyte | 0x01;
		else if(SDA == 0)
			readbyte = readbyte | 0x00;
	}
	SCL = LOW;
	return(readbyte);
}

void send_Data(unsigned char mData)
{
	unsigned short int x;
	bit sendBit;
	unsigned char MASK = 0x80;
	for(x=0;x<8;x++)
	{
		sendBit = mData & MASK;
		MASK >>= 1;

		SCL = LOW;
		SDA = sendBit;
		SCL = HIGH;
	}
	SCL = LOW;
	SDA = LOW;
}

void send_Device_Address(unsigned char address)
{
	send_Data(address);
}

void send_Word_Address(unsigned char address)
{
	send_Data(address);
}

void device_ACK(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
}

void Master_ACK(void)
{
	SCL = LOW;
	SDA = LOW;
	SCL = HIGH;
	SCL = LOW;
}

void set_Current_Address(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	I2C_Start();	
		send_Device_Address(Device_Address); // write mode
		device_ACK();
	
		send_Word_Address(byte1_Word_Address); // first byte of word address
		device_ACK();
	
		send_Word_Address(byte2_Word_Address); // second byte of word address
		device_ACK();
	I2C_Stop();	
}


