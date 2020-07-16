// Program for LCD

#include<reg51.h>
#include "LCD.h"	  // To include all declarations of functions
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;

void Initialize_LCD()
{
	P0=0xFF;
	P1=0xFF;
	P2=0xFF;
	Send_Command_LCD(0x38);
	Delay(5);
	Send_Command_LCD(0x0C);
	Delay(5);
	Clear_LCD();
	Delay(5);
}

void Send_Command_LCD(unsigned char comd)
{
	RS=0;
	RW=0;
	EN=1;
	P1=comd;
	Delay(5);
	EN=0;
}

void Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<100;v++);
}

void Display_Data_LCD(unsigned char *mdata)
{
	unsigned char s;
	for(s=0;mdata[s]!=0;s++)
	{
		Display_Char_LCD(mdata[s]);
	}
	
}

void Display_Char_LCD(unsigned char a)
{
	RS=1;
	EN=1;
	P1=a;
	Delay(100);
	EN=0;
}

void Clear_LCD()
{
	Send_Command_LCD(0x01);
	Delay(5);
	Send_Command_LCD(0x02);
	Delay(5);
}