#include <reg51.h>
#include "LCD.H"
#include "COMMON.H"
#include "Keydata.h"

void board_init(void)
{
	initialise_LCD();
}

void main(void)
{

	unsigned char i,myChar[6][8]={{31,16,31,01,31,00,00,00},
								{14,17,17,31,17,00,00,00},
								{31,16,16,16,31,00,00,00},
								{17,17,31,17,17,00,00,00},
								{31,04,04,04,31,00,00,00},
								{17,25,21,19,17,00,00,00}};
	board_init();
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


