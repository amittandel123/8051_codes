#include<reg51.h>
void delay(void);
void main(void)
{
	while(1)
	{
	 	P2=0x55;
		delay();
		P2=0xAA;
		delay();
	}
}

void delay()
{
	TMOD=0x01;
	TL0=0x10;
	TH0=0x00;
	TR0=1;				// turn ON timer
	while(TF0==0);	    // check status of overflow flag
	TR0=0;
	TF0=0;
}