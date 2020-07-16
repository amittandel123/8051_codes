// 50 mS Delay using Timer
#include<reg51.h>
void T0M1Delay(void);
void Sec_Delay();
void Minute_Delay();
void Hour_Delay();

sbit LED = P2^0;

void main(void)
{
	while(1)
	{
		LED = ~LED;
		Minute_Delay();
	}
}

void Sec_Delay()
{
	int i;
	for(i=0;i<13;i++)	// 1Sec Delay
		T0M1Delay();
}
void Minute_Delay()
{
	int i;
	for(i=0;i<780;i++)	// 1Minute Delay
		T0M1Delay();
}
void Hour_Delay()
{
	int i;
	for(i=0;i<60;i++)	// 60 Minutes Delay
		Minute_Delay();
}
/************************************************************************/
void T0M1Delay()
{
	TMOD = 0x01;	//Timer 0, mode 1 (16 bit)
	TL0 = 0xFD;		// 50mS Delay. FFFFh-4BFDh = B402h = 46082+1 = 46083
	TH0 = 0x4B;		// 46083 X 1.085uS = 50mS
	TR0 = 1;			// Turn ON timer
	while(TF0 == 0);	// wait for TF0 to roll over
	TR0 = 0;			// Turn OFF Timer
	TF0 = 0;			// reset TF0	
}
/************************************************************************
void T0M1Delay()
{
	TMOD = 0x01;	//Timer 0, mode 1 (16 bit)
	TL0 = 0x00;		// 50mS Delay. FFFFh-0000h = FFFFh = 65535+1 = 65536
	TH0 = 0x00;		// 65536 X 1.085uS = 71.1mS
	TR0 = 1;			// Turn ON timer
	while(TF0 == 0);	// wait for TF0 to roll over
	TR0 = 0;			// Turn OFF Timer
	TF0 = 0;			// reset TF0	
}
************************************************************************/