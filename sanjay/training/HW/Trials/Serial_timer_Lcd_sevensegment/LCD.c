#include <reg51.h>
#include <stdio.h>
#include <intrins.h>
// cursor movement
#define LEFT 0
#define RIGHT 1
int No_Of_Char_on_LCD=1;

bit LCD_8_BIT = 1;
bit BLINK = 0;
bit CURSOR = 0;
bit SHIFT_L = 0;
bit SHIFT_R = 0;

sbit RS = P2^6;
sbit EN = P2^5;
int multi_char_count;
void move_cursor(int, unsigned int);
void display_data(unsigned char port_data);
void multi_char_key(void);
void ClearDisplay(void);
void SendCommand(unsigned char comd);
void delay(unsigned int val);
void GenChar_at_CGRAM(unsigned char []);
unsigned char Lookup_table[]= {'0','1','2','3','4','5','6','7','8','9','.',65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122};

void initialise(void)
{
	P1 = 0xFF;
	P2 = 0xFF;
	P3=0xFF;
	EN=0;
	RS= 0;
}

void writeCommand_LCD(void)
{
	unsigned char Command;
	if(LCD_8_BIT==1)
		Command=0x38;
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


/*
void GenChar_at_CGRAM(unsigned char mdata[])
{
	int cnt;
	for(cnt=0;cnt<8;cnt++)
	{
		display_data(mdata[cnt]);
		delay(15);
	}
}
*/
void GenChar_at_CGRAM(unsigned char mdata[])
{
	int cnt;
	SendCommand(0x41);
	delay(5);
	for(cnt=0;cnt < 8;cnt++)
	{
		display_data(mdata[cnt]);
		delay(5);
	}
}


void Send(unsigned char comd)
{
	RS=0;  // to select Command Register
	P0 = comd;// put command on PORT2
	EN=1;// to Enable the LCD processor
	_nop_();
	EN=0;// to disable the LCD processor
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
	else if (LCD_8_BIT==0)//4 bit
	{
		Send(comd);
		delay(5);
		Send(16*(tmpcomd & 0x0F));
		delay(5);
	}
}

void ClearDisplay(void)
{
	SendCommand(0x01);// clear display
	delay(5);

	SendCommand(0x02);//send cursor to home location
	delay(5);
}

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
	else if(LCD_8_BIT==0)//4 bit
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
	RS=1;// to select DDRAM Register i.e. data register
	P0=port_data;// put data on port
	EN=1;// to Enable the LCD processor
	_nop_();
	EN=0;// to disable the LCD processor
}

void delay(unsigned int val)
{
	unsigned int u,v;
	for(u=0;u<=val;u++)
		for(v=0;v<=100;v++);
}

void main(void)
{
	unsigned char i,myChar[6][8]={{31,16,31,01,31,00,00,00},
								{14,17,17,31,17,00,00,00},
								{31,16,16,16,31,00,00,00},
								{17,17,31,17,17,00,00,00},
								{31,04,04,04,31,00,00,00},
								{17,25,21,19,17,00,00,00}};
	initialise();
	writeCommand_LCD();
	SendCommand(0x80);// select line 1 on LCD
	delay(5);

	writeData_LCD("**** Sachin ****",16);
		
	SendCommand(0xC0);// select line 2 n LCD
	delay(5);
	
	writeData_LCD(" Sachin bamane",14);
	P0=0xFF;
	SendCommand(0x02);
	delay(5);

	while(1)
	{
//		if(P0!=0xFF)
		{
//			if(P0==0x7F)
			{
				for(i=0;i<8;i++)
				{
					SendCommand(0x80 + i);// select line 1 on LCD
					delay(5);
					display_data(i);
					delay(5);
				}
			}	
		}
	}
}
void multi_char_key(void)
{
 	writeData_LCD(&(Lookup_table[multi_char_count]),1);
 	multi_char_count++;
	move_cursor(LEFT,0);
	if(multi_char_count>63)
		multi_char_count=0;
	delay(5);
}
void move_cursor(int Direction, unsigned int ChangeCount)
{
	if (Direction==LEFT)
	{
		if(ChangeCount==1)
		{
			multi_char_count=0;
			No_Of_Char_on_LCD--;
		}
		if(No_Of_Char_on_LCD==40)
		{	
			No_Of_Char_on_LCD=16;
			SendCommand(0x8F);
		}
		else if(No_Of_Char_on_LCD==0)
		{	
			No_Of_Char_on_LCD=80;
			SendCommand(0xCF);
		}
		else
		{
			SendCommand(0x10);
		}
	}
	else if (Direction==RIGHT)
	{
		if(ChangeCount==1)
		{
			multi_char_count=0;
			No_Of_Char_on_LCD++;
		}
		
		if(No_Of_Char_on_LCD==17)
		{	
			No_Of_Char_on_LCD=41;
			SendCommand(0xC0);
		}
		else if(No_Of_Char_on_LCD==81)
		{	
			No_Of_Char_on_LCD=1;
			SendCommand(0x80);
		}
		else
		{
			SendCommand(0x14);
		}
	}
	delay(500);
}

