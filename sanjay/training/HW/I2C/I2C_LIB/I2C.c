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
	#pragma ASM
		MOV R7, #0x00
		MOV R6, #0x08
	READ_LOOP:
		CLR SCL
		SETB SCL

		CLR C
//		MOV A, R7
		RL A
		MOV R7, A

		JNB SDA, LINE_IS_CLR
		MOV A, R7
		ORL A, #0x01
		MOV R7, A
	LINE_IS_CLR:	
		DJNZ R6, READ_LOOP
		CLR SCL
	#pragma ENDASM
	
/*	unsigned int i;
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
*/
}

void send_Data(unsigned char mData)
{
	#pragma ASM
		MOV A, R7 // store command
		MOV R5,A
		MOV R7, #0x00
		MOV R6, #0x08
	WRITE_LOOP:
		CLR C
		CLR SCL
		CLR SDA
		MOV A, R5
		RLC A
		MOV R5, A
		JNC CLR_LINE
		SETB SDA
	CLR_LINE:
		SETB SCL
		DJNZ R6, WRITE_LOOP
		CLR SCL
		CLR SDA
	#pragma ENDASM

/*	unsigned char x;
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
*/
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
