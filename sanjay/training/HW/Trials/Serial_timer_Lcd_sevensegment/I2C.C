#include <reg51.h>
#include <intrins.h>
#include "I2C.H"

#define HIGH 1
#define LOW  0

sbit SDA 	= P1^0;
sbit SCL 	= P1^1;

void init_I2C(void)
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
	unsigned char x;
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

void device_ACK(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
}

void Master_NO_ACK(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
	SCL = LOW;
}

