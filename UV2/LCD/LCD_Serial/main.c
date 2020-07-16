/*******************************************************************************************
This program interfaces LCD & Serial port with 8051. 
First msg of 'LCD is Ready' is flashed on LCD then we can send data from keyboard to LCD
through comp port using hyper terminal(Baud rate is 4800)

********************************************************************************************/

#include<reg51.h>

#include"LCD.h"			  // To include all declarations of functions
#include"Serial.h"		  // To include all declarations of functions

unsigned char u, recd_byte;
void main()
{
	Initialize_LCD();
	Send_Command_LCD(0x80);
	Delay(5);
	Display_Data_LCD("LCD is Ready",12);
	Delay(5);
	Send_Command_LCD(0xC0);
	Delay(5);
	Serial_Init();
	for(u=0;u<17;u++)
	{
		recd_byte=Serial_Receive();
		Display_Char_LCD(recd_byte);	
		Delay(5);
	}
}