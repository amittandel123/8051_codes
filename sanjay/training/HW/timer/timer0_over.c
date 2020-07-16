#include <stdio.h>
#include <reg52.h>
#include "Keydata.h"
#include "I2CDATA.H"

void initilise();
unsigned int tick_count, timer0_flag,tick_count1,timer1_flag,sec,sec1;
/* void main()
{
	int x;
	initilise();
	x = 0;
	while(1)
	{
		TR0 = 1;  // run timer0
		TR1 = 1; // run timer1;
  	
		if((timer0_flag == 1)|| (timer1_flag == 1))
		{
			timer0_flag = 0;
			tick_count = 0;
		 	sec++;
			printf("%d",sec);
			
			timer1_flag = 0;
			tick_count1 = 0; 
			sec1++;
			printf(" \t%d \n",sec1);

		}

	 // if sec are 60, then 1min is completed.
		if((sec == 60) || (sec1 == 60))                  
		{
			x++;	
			printf("\n %d min. is over", x);
			sec = 0; sec1 = 0;
		}
	}
} */

void initilise()
{
	
// timer0 used for counting and timer1 used to printf the statements
/*  TMOD = 0x21;    //mode 2 for timer1 and mode 1 for timer 0.

	SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr */
 //	TH1   = 253;                       /* TH1:  reload value for 9600 baud @ 11.059 MHz */
// 	TR1   = 1;                          /* TR1:  timer 1 run */
// 	TI    = 1;  */

//timer0 used for counting and timer2 used to printf the statements	
//	TMOD = 0x01;				//mode 1 for timer 0.
	SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr */
	T2CON  = 0x30;		        /* SCON: mode 1, 8-bit UART, enable rcvr */
	TH2 = 0xFF;
	TL2 = 0xDC;
	RCAP2H = 0xFF;                       /* TH1:  reload value for 9600 baud @ 11.059 MHz */
 	RCAP2L = 0xDC;
	TR2   = 1;                          /* TR1:  timer 1 run */
    TI    = 1;  

// timer0 & timer1 used for counting and timer2 used to printf the statements	

	TMOD = 0x11; //mode 1 is selected for timer0 and timer1

	//5ms delay
	TH1 = 0xEC;   //as per 11.059MHZ clock
	TL1 = 0x78;
	timer1_flag = 0;

	// 1ms delay
	TH0 = 0xFC;   //as per 11.059MHZ clock
	TL0 = 0x2B;
	timer0_flag = 0;
	
	ET0 = 1;
	ET1 = 1;
	EA = 1;
//	TR0 = 1;
	tick_count = tick_count1 =sec = sec1= 0;
}

void timer0(void) interrupt 1
{
	TR0 = 0;
	TH0 = 0xFC;
	TL0 = 0x2B;
//	TR0 = 1;

	tick_count++;

	if(tick_count == 900)   // for creating 1sec delay, call 1ms 
		timer0_flag = 1; 	// delay approx. 900 times.
}

void timer1(void) interrupt 3
{
	TH1 = 0xEC;   
	TL1 = 0x78;
	TR1 = 0;

	tick_count1++;
	
	if(tick_count1 == 180) // for creating 1sec delay, call 1ms 
		timer1_flag = 1;   // delay approx. 180 times.
 
}
