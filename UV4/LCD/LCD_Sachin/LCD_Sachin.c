#include <reg51.h>
bit LCD_8_BIT = 0;
sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
void display_data(unsigned char port_data);
void ClearDisplay(void);
void SendCommand(unsigned char comd);
void delay(unsigned int val);

void initialise(void)
{
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EN = 0;
	RS = 0;
	RW = 0;
}

void writeCommand_LCD(void)
{
	unsigned char Command;
	if(LCD_8_BIT==1)
		Command=0x3C;
	else if(LCD_8_BIT==0)
		Command=0x28;
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

void Send(unsigned char comd)
{
	RS=0;  					// to select Command Register
	RW = 0;
	P1 = comd;					// put command on PORT2
	EN=1;						// to Enable the LCD processor
	delay(5);
	EN=0;						// to disable the LCD processor
}
void SendCommand(unsigned char comd)
{
	unsigned char tmpcomd;
	tmpcomd=comd;
	if(LCD_8_BIT==1)
	{	
		Send(comd);
		delay(5);
	}
	else if (LCD_8_BIT==0)				//4 bit
	{
		Send(comd);
		delay(5);
		Send(16*(tmpcomd & 0x0F));
		delay(5);
	}
}

void ClearDisplay(void)
{
	SendCommand(0x01);				// clear display
	delay(5);
   	SendCommand(0x02);				//send cursor to home location
	delay(5);
}

void writeData_LCD(unsigned char mdata[], unsigned int counter)
{
	unsigned char tmpdata;
	unsigned int cnt;
	if(LCD_8_BIT==1)
	{	
		for(cnt=0;cnt<counter;cnt++)
		{
			display_data(mdata[cnt]);
			delay(20);
		}
	}
	else if(LCD_8_BIT==0)				//4 bit
	{
		for(cnt=0;cnt<counter;cnt++)
		{	
			tmpdata=mdata[cnt];
			display_data(tmpdata);
			delay(20);
			display_data(16*(tmpdata & 0x0F));
			delay(20);
		}
	}
}

void display_data(unsigned char port_data)
{
	RS=1;						// to select DDRAM Register i.e. data register
	RW = 0;
	P1=port_data;					// put data on port
	EN=1;						// to Enable the LCD processor
	delay(50);
	EN=0;						// to disable the LCD processor
}

void delay(unsigned int val)
{
	unsigned int u,v;
	for(u=0;u<=val;u++)
		for(v=0;v<=100;v++);
}

void main(void)
{
	initialise();
	writeCommand_LCD();
	SendCommand(0x80);				// select line 1 on LCD
	delay(5);
	writeData_LCD("**** E-LAB ****",15);
	SendCommand(0xC0);				// select line 2 on LCD
	delay(5);
	writeData_LCD(" HAPPY DIWALI ",14);
	ClearDisplay();
}