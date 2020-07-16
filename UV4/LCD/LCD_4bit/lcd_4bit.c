/*********************************************************
 * Program to interface 16 X 2 LCD with 8051
 *  Created on: Apr 28, 2017
 *  Author: Amit Tandel
 *
 *	P1.0 = RS
 *	P1.1 = RW
 *	P1.2 = EN
 *
 *	P1.3 = LCD DATA PIN D4
 *	P1.4 = LCD DATA PIN D5
 *	P1.5 = LCD DATA PIN D6
 *	P1.6 = LCD DATA PIN D7
 *
 *********************************************************/

#include<reg51.h>
	  
sbit RS = P3^2;
sbit EN = P3^3;

sbit D4 = P2^4;
sbit D5 = P2^5;
sbit D6 = P2^6;
sbit D7 = P2^7;

#define RS_High  (RS = 1)
#define RS_Low   (RS = 0)
//#define RW_High  (RW = 1)
//#define RW_Low   (RW = 0)
#define EN_High  (EN = 1)
#define EN_Low   (EN = 0)

void Initialize_LCD();
void Pulse_LCD();
void Send_Command_LCD(unsigned char );
void SendByte(unsigned char );
void Delay(unsigned int );
void Display_Data_LCD(unsigned char *, unsigned char );
void Display_Char_LCD(unsigned char );
void Clear_LCD();
void Reset_LCD();

void Initialize_LCD()
{
	P1 = 0xFF;	// output
	P2 = 0xFF;	// input
	P3 = 0xFF;	// output
	EN = 0;
	RS = 0;	// command mode
//	RW = 0;	// write mode

//	Reset_LCD();
	Send_Command_LCD(0x28);	// 4-bit interface, 2 lines, 5X7 Pixels
	Delay(50);
	Send_Command_LCD(0x28);	// 4-bit interface, 2 lines, 5X7 Pixels
	Delay(50);
	Send_Command_LCD(0x0E);	// Turn on visible underline cursor
	Delay(50);
	Clear_LCD();
	Delay(50);
	Send_Command_LCD(0x80);		// Set Cursor at 1st Line
	Delay(50);
	Display_Data_LCD("WATER   CHEMICAL",16);
	Delay(50);
	Send_Command_LCD(0xC0);		// Set Cursor at 2nd Line
	Delay(50);
	Display_Data_LCD("AMIT TANDEL",11);
	Delay(50);
}

void Pulse_LCD()	// Function to toggle EN pin frm High to Low
{
	EN_High;
	Delay(10);
	EN_Low;
	Delay(10);
}

/*
 * In 4-Bit mode the higher byte of data is sent followed by lower byte.
 */

void Send_Command_LCD(unsigned char Data)
{
	unsigned char tempdata;
	RS_Low;
	RS_Low;
	tempdata = ((Data>>4) & 0x0F);	// Masking Higher bytes of _data
	SendByte(tempdata);
	Pulse_LCD();

	tempdata = (Data & 0x0F);	// Masking Lower byte of _data
	SendByte(tempdata);
	Pulse_LCD();
}

void SendByte(unsigned char mydata) // Function for masking data bits
{
	if(mydata & 0x01)
		D4 = 1;
	else
		D4 = 0;
	Delay(1);

	if(mydata & 0x02)
		D5 = 1;
	else
		D5 = 0;
	Delay(1);

	if(mydata & 0x04)
		D6 = 1;
	else
		D6 = 0;
	Delay(1);

	if(mydata & 0x08)
		D7 = 1;
	else
		D7 = 0;
	Delay(1);
}

void Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<100;v++);
}

void Display_Data_LCD(unsigned char *mdata, unsigned char count)
{
	unsigned char s;
	RS_High;
	RS_High;
	for(s=0;s<count;s++)
	{
		unsigned char tempdata;
		tempdata =((*mdata>>4) & 0x0F);	// Shifted Higher bits to right
		SendByte(tempdata);				// & masked to send command
		Pulse_LCD();

		tempdata = (*mdata & 0x0F);	// Masking & sending lower bit
		SendByte(tempdata);
		Pulse_LCD();
		mdata++;
	}
}

void Display_Char_LCD(unsigned char a)
{
	unsigned char temp;
	RS_High;
	RS_High;
	temp = ((a>>4) & 0x0F);
	SendByte(temp);
	Pulse_LCD();
	temp = (a & 0x0F);
	SendByte(temp);
	Pulse_LCD();
}

void Clear_LCD()
{
	Send_Command_LCD(0x01);	// Clears LCD
	Delay(50);
	Send_Command_LCD(0x02);	// Sets cursor position to home
	Delay(50);
}

void Reset_LCD()
{
	Send_Command_LCD(0x30);
	Delay(50);
	Send_Command_LCD(0x30);
	Delay(5);
	Send_Command_LCD(0x20);
}

void main()
{
	Delay(160);
	Initialize_LCD();
	while(1);
}