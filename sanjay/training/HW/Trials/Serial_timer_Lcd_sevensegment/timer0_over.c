#include <stdio.h>
#include <reg52.h>
#include "TIMER0.H"

void init_timer0();
unsigned int tick_count, timer0_flag,tick_count1,timer1_flag,sec,sec1;

void init_timer0(void)
{
	TMOD = 0x11; //mode 1 is selected for timer0 and timer1

	// 1ms delay
	TH0 = 0xFC;   //as per 11.059MHZ clock
	TL0 = 0x2B;
	timer0_flag = 0;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
	tick_count = sec = 0;
}

void timer0(void) interrupt 1
{
	TR0 = 0;
	TH0 = 0xFC;
	TL0 = 0x2B;
//	TR0 = 1;

	tick_count++;

	if(tick_count == 450)   // for creating 1sec delay, call 1ms 
		timer0_flag = 1; 	// delay approx. 900 times.
}

