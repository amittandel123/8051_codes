#include<reg51.h>

sbit LED=P0^0;
void Delay(unsigned int);
void main(void)
{
	TMOD=0x01;				/* Timer 0,Mode 1 */
	TH0=0x00;
	TL0=0x00;				/* Clear the sensor1 input */ 		        
	while(1)
	{
		LED = ~LED;
		Delay(10);
	}
}

void Delay(unsigned int value)	  /* Delay Subroutine*/
{     
	unsigned int i;
	for(i=0;i<=value;i++)
	{
		TR0=1;		  /* Start the timer */ 
		while(TF0==0);	  /* wait till overflow */ 
		TR0=0;		  /* Stop the timer */ 
		TF0=0;
	}
}