#include<reg51.h>
#include"iButton.h"

sbit LED_1 = P2^0;
sbit LED_2 = P2^1;

unsigned char sn[8],i;
void main()
{
	int Read_Reset;
	P1=0x00;
	LED_1 = 0;
	LED_2 = 0;
	
	while(1)
	{
		Read_Reset = Reset_iButton();
		Write_Byte(0x33);
		for(i=0;i<8;i++)     //loop 8 times to read the 64 bit ID	
		{ 
			sn[i] = Read_Byte(); 
		} 					// the array sn[] would hold the 64bit ID
		if(Read_Reset)
			LED_1=1;
		else
			LED_1=0;
	}
}
/******************************************************************************************************
for(;;)
{
	detect = 0; //paranoid check
	detect  = OWReset(); //reset the 1-wire bus
	OWWriteByte(0x33);   //issue a 1-wire command
	for(i=0;i<8;i++)     //loop 8 times to read the 64 bit ID	
	{ 
		sn[i] = OWReadByte(); 
	} 					// the array sn[] would hold the 64bit ID
	if(detect & 0x01)
	{ 
		onLED(2);
	}  //if a valid 1-wire device was detected light up
	else { offLED(2); }
}
******************************************************************************************************/
