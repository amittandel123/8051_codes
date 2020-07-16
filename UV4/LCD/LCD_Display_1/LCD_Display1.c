// New

#include<reg51.h>

sbit RS=P3^2;
sbit EN=P3^3;

#define LCD_PORT								P1
#define LCD_CursorHome1stLine		Send_Command(0x80)
#define LCD_CursorHome2ndLine		Send_Command(0xC0)
#define LCD_Blink								Send_Command(0x0E)


void Initialize();
void Display_Data(unsigned char mdata[], unsigned int count);
void Send_Command(unsigned char );
void Clear_LCD();
void Delay(unsigned int);
void Display_Char(unsigned char);

void Initialize()
{
	P0=0x00;		// P0 as O/P port
	P1=0x00;		// P1 as O/P port
	P2=0xFF;		// P2 as i/p port
	P3=0x00;		// P3 as O/P port
	
	Send_Command(0x38);	// LCD as 2 Line, 5x7 Matrix
	Delay(5);
	Send_Command(0x0C);	// Display ON, Cursor Blinking
	Delay(5);
	Clear_LCD();
	Delay(5);
}

void Send_Command(unsigned char comd)
{
	RS=0;
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
	}
}

void Display_Char(unsigned char c)
{
	RS=1;
	EN=1;
	P1 = c;
	Delay(100);
	EN=0;
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
	Send_Command(0x80);
	Delay(5);
	Display_Char('A');
	Delay(50);
	
	Send_Command(0xCE);
	Delay(5);
	Display_Char('A');
	Delay(50);
	while(1);
}




/*********************************************************************************
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
	Send_Command(0x80);
	Delay(5);
	Display_Data("**AMIT TANDEL**", 15);
	Delay(50);		
	Send_Command(0xC0);
	Delay(5);
	Display_Data("*Godrej Locks*", 14);
	Delay(500);

	Clear_LCD();

	Send_Command(0x80);
	Delay(5);
	Display_Data("**Wishes You**", 14);
	Delay(50);		
	Send_Command(0xC0);
	Delay(5);
	Display_Data("*HappyNewYear*", 14);
	Delay(500);
			 
}
*********************************************************************************/



/************** Alternate Way *********************
void lcd_data_string(unsigned char *str) // Function to send string on LCD
{
	int i=0;
	while(str[i]!='\0')
	{
		lcd_data(str[i]);
		i++;
		delay(10);
	}
}
*/