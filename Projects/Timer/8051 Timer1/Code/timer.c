#include<reg51.h>
sbit LED = P0^0;
void delay(unsigned int);
void main(void)
{
	TMOD=0x01;				/* Timer 0,Mode 1 */
	TH0=0xFC;
	TL0=0x67;				/* Clear the sensor1 input */ 
	
	while(1)
	{
		LED = ~LED;
		delay(100);
	}
}

void delay(unsigned int x)	  /* Delay Subroutine*/
{     
	unsigned int i;
	for(i=0;i<=x;i++)
	{
		TR0=1;		  /* Start the timer */ 
		while(TF0==0);	  /* wait till overflow */ 
		TR0=0;		  /* Stop the timer */ 
		TF0=0;
	}
}