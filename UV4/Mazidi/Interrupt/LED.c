/*
Program that continuously gets a single bit of data from P1.7
and sends it to P1.0, while simultaneously creating a square wave of
200 ìs period on pin P2.5. Use Timer 0 to create the square wave.
Assume that XTAL = 11.0592 MHz.

We will use timer 0 mode 2 (auto-reload). One half of the period is
100 ìs. 100/1.085 ìs = 92, and TH0 = 256 - 92 = 164 or A4H
*/

#include <reg51.h>
sbit SW =P1^7;
sbit IND =P1^0;
sbit WAVE =P2^5;
void timer0(void) interrupt 1 
{
	unsigned int x;
	WAVE=~WAVE; //toggle pin
	for(x=0;x<30000;x++);
}
void main() 
{
	SW=1; //make switch input
	TMOD=0x02;
	TH0=0xA4; //TH0=-92
	IE=0x82; //enable interrupt for timer 0
	while (1) 
	{
		IND=SW; //send switch to LED
	}
}