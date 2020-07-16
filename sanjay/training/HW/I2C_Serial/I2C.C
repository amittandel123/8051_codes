#include <reg51.h>
#include <intrins.h>

#define HIGH 1
#define LOW  0

sbit SDA 	= P1^0;
sbit SCL 	= P1^1;

//sbit WP 	= P1^2;
/*
void I2C_Reset(void)
{
	unsigned int j = 0,i = 0;
	while(1)
	{
		for(j=0;j<9;j++)
		{
			SCL = LOW;
			SCL = HIGH;
			if(SDA==HIGH)
				i++;
		}
		if(i<9)// memory not Resetted
			break;
	}
		
		
}
*/

void I2C_initialise(void)
{
//	WP 	= 0x00;
	SCL = HIGH;
	SDA = HIGH;
}

void I2C_Start(void)
{
//	WP 	= 0x00;
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
//	WP 	= 0x01;
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

/*
void send_Device_Address(unsigned char address)
{
	send_Data(address);
}

void send_Word_Address(unsigned char address)
{
	send_Data(address);
}
*/

void device_ACK(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
//	while(SDA == HIGH);
}

/*
void Master_ACK(void)
{
	SCL = LOW;
	SDA = LOW;
	SCL = HIGH;
	SCL = LOW;
}
*/

void Master_NO_ACK(void)
{
	SCL = LOW;
	SDA = HIGH;
	SCL = HIGH;
	SCL = LOW;
}

/*
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
*/
