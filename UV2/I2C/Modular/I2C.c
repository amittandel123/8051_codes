#include <reg51.h>
#include"I2C.h"

sbit SCL=P3^3;
sbit SDA=P3^5;	

void delay(void); 
extern void print(unsigned char mData[]);

void I2C_initialise(void)
{
	SCL=1;
	SDA=1;
}

void I2C_Start(void)
{ 
	SCL=0;
	SDA=1;
	SCL=1;
	SDA=0;
}

void I2C_Stop(void)
{
	SCL=0;
	SDA=0;
	SCL=1;
	SDA=1;
}

void device_ACK(void)
{
	SCL=0;
//	SDA=1;
	SCL=1;
//	SDA=0;
}

void Master_NO_ACK(void)
{
	SCL=0;
//	SDA=1;
	SCL=1;
//	SDA=0;
}

void send_Data(unsigned char str_data)
{
	unsigned char i,j=0x80;

	for(i=0;i<8;i++)
	{
		SCL=0;
		if(str_data & j)
			SDA=1;
		else
			SDA=0;
		j >>= 1;
		SCL=1;
	}
	SCL=0;
}

unsigned char Read_Data(void)
{
	unsigned char i=8;
	unsigned readbyte=0x00;
 
	while(i--)
	{
		readbyte = readbyte << 1;
		SCL=0;
		SCL=1;
		if(SDA==1)
		{
			readbyte = readbyte | 0x01;
		}
//		else if(SDA==0)
//		{
//			readbyte = readbyte | 0x00;
//		}
	}
	SCL=0;
	return readbyte;
}