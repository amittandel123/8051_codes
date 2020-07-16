#include<reg51.h>
sbit sensordrcls=P1^0;
sbit LED=P0^3;
unsigned int i,x;
void delay (unsigned int);
void main(void)
{
	unsigned char mydata[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	P2=0x00;				/* Made all cylinder to home position*/
	P0=0x00;				/* Initially no input to LED and counter */ 
	P1=0X00;				/* Initially no input to sensors */
	TMOD=0x01;				/* Timer 0,Mode 1 */
	TH0=0xFC;
	TL0=0x67;				/* Clear the sensor1 input */ 
							        
	while(1)                                 /* Do it continiously */ 					
	{	   
		if(sensordrcls==1)	 /* if the the door is closed 	*/
		{
			for(i=0;i<=5;i++)	                 /*  Press the code "1234*Enter"  */
			{
				P2=mydata[i];                    /* Cylinders will press the code  */
				delay(50);			 /*Delay between each Key press*/
			}
	
			if (sensordrcls==0)
			{		
				i=i++;
				P2=mydata[i];		      /* Close the door */
				i=i+1;
				delay(50);
				P2=0x00;				/* Cylinders back to original position */
				P2=mydata[i];			 /* Increment the number of count*/    
				delay(2);		         
				P2=0x00;
				delay(2);
			 }
			else
			{
				LED=1;
				while(1);
			}
		
		}		 	  
		else
		{
			LED=1;
			while(1);
		}
	}
}	

void delay(unsigned int x)	  /* Delay Subroutine*/
{     
	for(i=0;i<=x;i++)
	{
		TR0=1;		  /* Start the timer */ 
		while(TF0==0);	  /* wait till overflow */ 
		TR0=0;		  /* Stop the timer */ 
		TF0=0;
	}
}