/*
 * Program to Glow LEDs using 4bit Shift Register DM74LS95N
 */

#include<reg51.h>
#include<intrins.h>
#include<stdio.h>

sbit CLK=P1^0;
sbit DATA=P1^1;
sbit LED=P1^2;

void LED_Single_All_ON();
void LED_Single_All_OFF();
void LED_Row_wise_All_ON();
void LED_Row_wise_All_OFF();
void Clear();
void Pulse();
void Delay(unsigned int i);
unsigned int j;
void main()
{
	P1=0xFF;	// Make P1 o/p port
	Clear();
	while(1)
	{
		LED_Single_All_ON();
//		LED = 0;
		Delay(100);
//		LED = 1;
//		Delay(100);
//		Clear();
/*		
		LED_Single_All_OFF();
		Delay(50);
		LED_Row_wise_All_ON();
		Delay(50);
		LED_Row_wise_All_OFF();
		Delay(50);
*/
	}
}

void LED_Single_All_ON()	// One by one ON
{
	for(j=0;j<12;j++)	// 1 by 1 ON
	{
		DATA=1;
		Delay(1000);
		Pulse();		
	}
}

void LED_Single_All_OFF()	// One by one OFF
{
	for(j=0;j<12;j++)	// 1 by 1 OFF
	{
		DATA=0;
		Pulse();
		Delay(50);
	}
}

void LED_Row_wise_All_ON()
{
	for(j=0;j<12;j++)	// Row-wise ON
	{
		DATA=1;
		Pulse();
		if(j==3)
			Delay(50);
		else if(j==6)
			Delay(50);
		else if(j==9)
			Delay(50);
		else if(j==12)
			Delay(50);
	}
}

void LED_Row_wise_All_OFF()
{
	for(j=0;j<12;j++)	// Row-wise ON
	{
		DATA=0;
		Pulse();
		if(j==3)
			Delay(50);
		else if(j==6)
			Delay(50);
		else if(j==9)
			Delay(50);
		else if(j==12)
			Delay(50);
	}
}

void Clear()
{
	for(j=0;j<16;j++)
	{
		DATA=0;
		Pulse();
	}
}

void Pulse()
{
	CLK=1;
	_nop_();
//	Delay(1);
	CLK=0;
}

void Delay(unsigned int i)
{
	unsigned int u,v;
	for(u=0;u<i;u++)
		for(v=0;v<1275;v++);
}
