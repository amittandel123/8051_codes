#include <reg51.h>
#include "declare.h"

void wait(int time)
{
	time *=1000;
	while(time--)
	{
		_nop_();
	}
}

/*
void Sequential_Read(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address, unsigned int num_byte2read)
{
	unsigned int i=8,j;
	unsigned char x=8;
	unsigned char readbyte = 0x00;
	I2C_Start();	
	send_Device_Address(Device_Address); // write mode
	device_ACK();

	send_Word_Address(byte1_Word_Address); // first byte of word address
	device_ACK();

	send_Word_Address(byte2_Word_Address); // second byte of word address
	device_ACK();

	I2C_Start();	
		Device_Address = (unsigned char)(Device_Address + 1);
		send_Device_Address(Device_Address); // write mode
		device_ACK();
		while(num_byte2read--)
		{
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
			printf("%c",readbyte);
			SCL = LOW;
			SDA = LOW;
			SCL = HIGH;
			SCL =LOW;
//			if(num_byte2read!=1)
//				Master_ACK();
		}
		SCL = LOW;
		SDA = HIGH;
		SCL = HIGH;
	I2C_Stop();
}
*/
void initialise(void)
{
	P0 =0xFF;
	P1 = 0xFF;

	SCON	= 0x50;
	TMOD	= 0x20;
	TI	= 1;
	TR1	= 1;
	TH1	= 0xFD;
	EA	= 0;
}

unsigned char current_address_read(unsigned char Device_Address)
{
	int i=8;
	unsigned char x=8;
	unsigned char readbyte=0x00;
	Device_Address = (unsigned char)(Device_Address + 1);
	I2C_Start();	
		send_Device_Address(Device_Address); // write mode
		device_ACK();

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
		printf("%c",readbyte);

	I2C_Stop();	
	return(readbyte);
}

void byte_write(unsigned char byte,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	I2C_Start();	
		send_Device_Address(Device_Address); // write mode
		device_ACK();

		send_Word_Address(byte1_Word_Address); // first byte of word address
		device_ACK();

		send_Word_Address(byte2_Word_Address); // second byte of word address
		device_ACK();

		send_Data(byte);			
		device_ACK();
	I2C_Stop();	
}

unsigned char Random_Read(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	int i=8;
	unsigned char x=8;
	unsigned char readbyte = 0x00;
	I2C_Start();	
	send_Device_Address(Device_Address); // write mode
	device_ACK();

	send_Word_Address(byte1_Word_Address); // first byte of word address
	device_ACK();

	send_Word_Address(byte2_Word_Address); // second byte of word address
	device_ACK();

	I2C_Start();	
	Device_Address = (unsigned char)(Device_Address + 1);
		send_Device_Address(Device_Address); // write mode
		device_ACK();

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
		printf("%c",readbyte);

	I2C_Stop();	
	return(readbyte);
}

void main(void)
{
	unsigned char x = 0,y;
	initialise();
	I2C_initialise();	
	y=0x18;	
	while(1)
	{
//		byte_write(65 + x,0xA0,0x00,y); // (data,Dev_add,word1,word2)
		x++;
		wait(10);
//		set_Current_Address(0xA0,0x00,0x00);  // (Dev_add,word1,word2)
//		x = current_address_read(0xA0);
		x = Random_Read(0xA0,0x00, y);
//		Sequential_Read(0xA0, 0x00, y, 8);
		y++;
//		if(y>31)
//			while(1);
		printf("Data Read : %c", x);
	}
}


