/*
	Program to create Delay of mSec, Sec & Minutes.
	Crystal Freq = 11.0592MHz
	uC : P89V51RD2
*/

#include<reg51.h>

sbit LED1 = P2^1;
sbit LED2 = P2^2;
sbit LED3 = P2^3;

void mSec_Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<108;v++);
}
void Sec_Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<100;v++);
}
void Minute_Delay(unsigned int value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<1275;v++);
}
void main()
{
	P2 = 0x00;	// P2 as Output port
	while(1)
	{
		LED1 = 1;
		mSec_Delay(1000);
		LED1 = 0;
		mSec_Delay(1000);
	}
}