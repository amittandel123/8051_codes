#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
#define LCD_DATA_PORT P0
bit LCD_8_BIT = 1;
sbit RS = P2^6;
sbit EN = P2^5;
void display_data(unsigned char port_data);
void ClearDisplay(void);
void SendCommand(unsigned char comd);
void delay(unsigned int val);
void GenChar_at_CGRAM(unsigned char mData[]);

void writeData_LCD(unsigned char mdata[], unsigned int counter)
{
	unsigned char tmpdata;
	unsigned int cnt;
	if(LCD_8_BIT==1)
	{	for(cnt=0;cnt < counter;cnt++)
		{
			display_data(mdata[cnt]);
			delay(5);
		}
	}
	else if(LCD_8_BIT==0)			//4 bit
	{
		for(cnt=0;cnt < counter;cnt++)
		{	
			tmpdata=mdata[cnt];
			display_data(tmpdata);
			delay(5);
			display_data(16*(tmpdata & 0x0F));
			delay(5);
		}
	}
}

void display_data(unsigned char port_data)
{
	RS=1;					// to select DDRAM Register i.e. data register
	LCD_DATA_PORT=port_data;		// put data on port
	EN=1;					// to Enable the LCD processor
	_nop_();
	EN=0;					// to disable the LCD processor
}


void initialise(void)
{
	P1 = 0xFF;
	LCD_DATA_PORT = 0xFF;
	P3=0xFF;
	EN=0;
	RS=0;
}

void delay(unsigned int val)
{
	unsigned int u,v;
	for(u=0;u<=val;u++)
		for(v=0;v<=100;v++);
}

void SendCommand(unsigned char comd)
{
	RS=0;  					// to select Command Register
	LCD_DATA_PORT = comd;			// put command on PORT2
	EN=1;					// to Enable the LCD processor
	_nop_();
	EN=0;					// to disable the LCD processor
}

void ClearDisplay(void)
{
	SendCommand(0x01);			// clear display
	delay(5);

	SendCommand(0x02);			//send cursor to home location
	delay(5);
}

void writeCommand_LCD(void)
{
	unsigned char Command;
	Command=0x38;

	delay(5);
	SendCommand(Command);

	delay(5);
	SendCommand(Command);


	delay(5);
	SendCommand(0x0C);

	delay(5);
	ClearDisplay();
	delay(5);
}

void main(void)
{
	initialise();
	writeCommand_LCD();

	SendCommand(0xC0);			// select line 2 on LCD
	delay(5);
	writeData_LCD("**** Embedd ****",16);
	delay(5);
	SendCommand(0x80);			// select line 1 on LCD
	delay(5);
	writeData_LCD("**** Sachin ****",16);
	delay(5);
	while(1)
	{
	}
}

