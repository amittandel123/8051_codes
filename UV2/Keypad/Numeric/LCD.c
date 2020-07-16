#include<reg51.h>

#define LCD_Port P1
sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
void LCD_Init();
void LCD_Command(unsigned char cmnd);
void LCD_Display(unsigned char num);
void Delay(unsigned char del);

void LCD_Display(unsigned char num)
{
	P1=0xFF;
	P2=0xFF;
	LCD_Init();
	RS=1;
	RW=0;
	EN=1;
	LCD_Port=num;
	Delay(10);
	EN=0;
}

void LCD_Init()
{
	LCD_Command(0x38);		// To select 2 lines with 5X7 matrix
	Delay(10);
	LCD_Command(0x0E);		// Display ON, Cursor blinking
	Delay(10);
	LCD_Command(0x06);		// Increment cursor(rightward)
	Delay(10);
	LCD_Command(0x01);		// Clear Display
	Delay(10);
}

void LCD_Command(unsigned char cmnd)
{
	RS=0;
	RW=0;
	EN=1;
	LCD_Port=cmnd;
	Delay(10);
	EN=0;
}
void Delay(unsigned char del)
{
	unsigned char u,v;
	for(u=0;u<del;u++)
		for(v=0;v<1275;v++);
}

