/*******************************************************************************************
This program interfaces LCD & Serial port with 8051. 
First msg of 'LCD is Ready' is flashed on LCD then we can send data from keyboard to LCD
through comp port using hyper terminal(Baud rate is 4800)

********************************************************************************************/

#include<reg51.h>

#include"LCD.h"			  // To include all declarations of functions
#include"Serial.h"		  // To include all declarations of functions
#include"I2C.h"
#include"EEPROM.h"

unsigned char u, recd_byte;
void main()
{
	Initialize_LCD();
	I2C_initialise();
	Send_Command_LCD(0x80);
	Delay(5);
	Display_Data_LCD("Sent Data: ",11);
	Delay(5);
	Send_Command_LCD(0xC0);
	Display_Data_LCD("Recd Data: ",11);
	Delay(5);

	while(1)
	{
		Send_Command_LCD(0x8A);
		I2C_Write("amit", 0x00,0x01);		
		Delay(5000);
		Send_Command_LCD(0xCA);
		recd_byte=I2C_Read(0x00, 0x01);
		Display_Char_LCD(recd_byte);
		
	}
}