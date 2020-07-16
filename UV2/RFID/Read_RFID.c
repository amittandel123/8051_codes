#include<reg51.h>
#include"LCD.h"
#include"Serial.h"

unsigned char u, recd_byte;
void main()
{
	Initialize_LCD();
	Send_Command_LCD(0x80);				// Set cursor at home location
	Delay(5);
	Display_Data_LCD("LCD is Ready");	// Message for testing LCD
	Delay(5);
	Send_Command_LCD(0xC0);
	Delay(5);
	Serial_Init();
	while(1)
	{
		recd_byte=Serial_Receive();
		for(u=0;u<12;u++)
		{
			Display_Char_LCD(recd_byte);	
			Delay(5);
		}
	}

}
