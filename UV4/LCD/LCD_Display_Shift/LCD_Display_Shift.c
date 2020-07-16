// New
#include<reg51.h>
sbit RS=P3^2;
sbit EN=P3^3;
void Initialize();
//void Display_Data(unsigned char mdata[], unsigned int count);
void Send_Command(unsigned char comd);
void Clear_LCD();
void Delay(unsigned int);
void Initialize()
{
	Send_Command(0x38);
	Delay(50);
	Send_Command(0x0C);
	Delay(50);
	Send_Command(0x1C);
	Delay(50);
	Clear_LCD();
	Delay(50);
}

void Send_Command(unsigned char comd)
{
	RS=0;
	EN=1;
	P1=comd;
	Delay(50);
	EN=0;
}

void Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<100;v++);
}

void Display_Data(unsigned char mdata[], unsigned int count)
{
	unsigned char x;
	for(x=0;x<count;x++)
	{
		RS=1;
		EN=1;
		P1= mdata[x];
		Delay(100);
		EN=0;
		Send_Command(0x18);	 // To shift display from Right to Left
		Delay(50);
	}
	
}

void Clear_LCD()
{
	Send_Command(0x01);
	Delay(5);
	Send_Command(0x02);
	Delay(5);
}

void main()
{
	Initialize();
	Send_Command(0x8F);		// To start 1st letter from last matrix of 1st line of LCD
	Delay(50);
	Display_Data("**AMIT TANDEL**", 15);
	Delay(500);		
	Send_Command(0xCF);		// To start 1st letter from last matrix of 2nd line of LCD
	Delay(50);
	Display_Data("*Godrej Locks*", 14);
	Delay(500);

	Clear_LCD();

	Send_Command(0x8F);
	Delay(50);
	Display_Data("**Wishes You**", 14);
	Delay(500);		
	Send_Command(0xCF);
	Delay(50);
	Display_Data("*HappyNewYear*", 14);
	Delay(500);
}



/****************************************************************************
// Original

#include<reg51.h>
sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P2^2;
void Initialize();
//void Display_Data(unsigned char mdata[], unsigned int count);
void Send_Command(unsigned char comd);
void Clear_LCD();
void Delay(unsigned int);
void Initialize()
{
	P0=0xFF;
	P1=0xFF;
	P2=0xFF;
	Send_Command(0x38);
	Delay(5);
	Send_Command(0x0C);
	Delay(5);
	Send_Command(0x1C);
	Delay(5);
	Clear_LCD();
	Delay(5);
}

void Send_Command(unsigned char comd)
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

void Display_Data(unsigned char mdata[], unsigned int count)
{
	unsigned char x;
	for(x=0;x<count;x++)
	{
		RS=1;
		EN=1;
		P1= mdata[x];
		Delay(100);
		EN=0;
		Send_Command(0x18);	 // To shift display from Right to Left
		Delay(5);
	}
	
}

void Clear_LCD()
{
	Send_Command(0x01);
	Delay(5);
	Send_Command(0x02);
	Delay(5);
}

void main()
{
	Initialize();
	Send_Command(0x8F);		// To start 1st letter from last matrix of 1st line of LCD
	Delay(5);
	Display_Data("**AMIT TANDEL**", 15);
	Delay(50);		
	Send_Command(0xCF);		// To start 1st letter from last matrix of 2nd line of LCD
	Delay(5);
	Display_Data("*Godrej Locks*", 14);
	Delay(500);

	Clear_LCD();

	Send_Command(0x8F);
	Delay(5);
	Display_Data("**Wishes You**", 14);
	Delay(50);		
	Send_Command(0xCF);
	Delay(5);
	Display_Data("*HappyNewYear*", 14);
	Delay(500);			 
}
****************************************************************************/
