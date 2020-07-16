#include<reg51.h>
#include "LCD.h"

sbit Row1=P0^0;
sbit Row2=P0^1;
sbit Row3=P0^2;
sbit Row4=P0^3;
sbit Col1=P0^4;
sbit Col2=P0^5;
sbit Col3=P0^6;

void Keypad();
void Display(unsigned char val);
void Scan_Col3();
void Scan_Col2();
void Scan_Col1();

void Keypad()
{
	P0=0x00;
	while(1)
	{
		Col1=Col2=Col3=1;   		// Make column pins i/p
		Row1=Row2=Row3=Row4=0;	// Make Row pins o/p
		if(Col1==0)
			Scan_Col1();
		else if(Col2==0)
			Scan_Col2();
		else if(Col3==0)
			Scan_Col3();
	}
}

void Scan_Col1()
{
	Row1=Row2=Row3=Row4=1;
	Row1=0;
	if(Col1==0)
		Display(1);
	Row1=1;
	Row2=0;
	if(Col1==0)
		Display(4);
	Row2=1;
	Row3=0;
	if(Col1==0)
		Display(7);
	Row3=1;
	Row4=0;
	if(Col1==0)
		Display(10);
	Row4=1;
}

void Scan_Col2()
{
	Row1=Row2=Row3=Row4=1;
	Row1=0;
	if(Col2==0)
		Display(2);
	Row1=1;
	Row2=0;
	if(Col2==0)
		Display(5);
	Row2=1;
	Row3=0;
	if(Col2==0)
		Display(8);
	Row3=1;
	Row4=0;
	if(Col2==0)
		Display(0);
	Row4=1;
}

void Scan_Col3()
{
	Row1=Row2=Row3=Row4=1;
	Row1=0;
	if(Col3==0)
		Display(3);
	Row1=1;
	Row2=0;
	if(Col3==0)
		Display(6);
	Row2=1;
	Row3=0;
	if(Col3==0)
		Display(9);
	Row3=1;
	Row4=0;
	if(Col3==0)
		Display(11);
	Row4=1;
}

void Display(unsigned char val)
{
	switch(val)
	{
		case 1: LCD_Display(1);
				break;
		case 2: LCD_Display(2);
				break;
		case 3: LCD_Display(3);
				break;
		case 4: LCD_Display(4);
				break;
		case 5: LCD_Display(5);
				break;
		case 6: LCD_Display(6);
				break;
		case 7: LCD_Display(7);
				break;
		case 8: LCD_Display(8);
				break;
		case 9: LCD_Display(9);
				break;
		case 0: LCD_Display(0);
				break;
		case 10: LCD_Display('*');
				break;
		case 11: LCD_Display('#');
				break;
	}
}