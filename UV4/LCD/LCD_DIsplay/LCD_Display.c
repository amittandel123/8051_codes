
#include<reg51.h>
void LCD_INIT();
void LCD_CLR();
void LCD_WRITE();
void DELAY(unsigned int);
sbit RS=P3^2;
sbit EN=P3^3;
unsigned char a;			 
unsigned char DATA[]=("HAPPY DIWALI");

void main()
{
	LCD_INIT();
	LCD_CLR();
	LCD_WRITE();
	while(1);
}
void LCD_INIT()
{

	RS=0;
	EN=1;
	P1=0x01;
	DELAY(10);
	EN=0;
	P1=0x38;
	EN=1;
	DELAY(10);
	EN=0;
	P1=0x0C;
	EN=1;
	DELAY(10);
	EN=0;
	P1=0x03;
	EN=1;
	DELAY(10);
	EN=0;
	DELAY(10);
	P1=0x01;
	EN=1;
	DELAY(10);
	EN=0;
	DELAY(10);
}

void LCD_CLR()
{
	RS=0;
	EN=1;
	P1=0x01; 	
	DELAY(10);
	EN=0;
	DELAY(10);
}

void LCD_WRITE()
{
	RS=1;
	for(a=0;a<12;a++)
	{
		P1=DATA[a];
		EN=1;
		DELAY(200);
		EN=0;
	}
	DELAY(20);
}

void DELAY(unsigned int x)
{
	unsigned int y,z;
	for(y=0;y<x;y++)
	{
		for(z=0;z<100;z++);
	}
}




/******************************************************************
#include<reg51.h>
void LCD_INIT();
void LCD_CLR();
void LCD_WRITE();
void DELAY(unsigned int);
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;
unsigned char a;			 
unsigned char DATA[]=("HAPPY DIWALI");

void main()
{
	LCD_INIT();
	LCD_CLR();
	LCD_WRITE();
	while(1);
}
void LCD_INIT()
{
	RW=0;
	RS=0;
	EN=1;
	P1=0x01;
	DELAY(10);
	EN=0;
	P1=0x38;
	EN=1;
	DELAY(10);
	EN=0;
	P1=0x0C;
	EN=1;
	DELAY(10);
	EN=0;
	P1=0x03;
	EN=1;
	DELAY(10);
	EN=0;
	DELAY(10);
	P1=0x01;
	EN=1;
	DELAY(10);
	EN=0;
	DELAY(10);
}

void LCD_CLR()
{
	RS=0;
	EN=1;
	P1=0x01; 	
	DELAY(10);
	EN=0;
	DELAY(10);
}

void LCD_WRITE()
{
	RS=1;
	for(a=0;a<12;a++)
	{
		P1=DATA[a];
		EN=1;
		DELAY(200);
		EN=0;
	}
	DELAY(20);
}

void DELAY(unsigned int x)
{
	unsigned int y,z;
	for(y=0;y<x;y++)
	{
		for(z=0;z<100;z++);
	}
}

/*
void DELAY()
{
	EN=0;
	RS=0;
	RW=1;
	P1=0xFF;
	EN=1;
	P1=1;
	if(P1^7==1);
	EN=0;
	RW=0;
}	 
*******************************************************************/
