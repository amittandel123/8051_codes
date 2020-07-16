// 50 mS Delay using Timer
#include<reg51.h>
void T0M1Delay(void);

sbit LED = P2^0;

void main(void)
{
	while(1)
	{
		LED = ~LED;
		T0M1Delay();
	}
}

void T0M1Delay()
{
	TMOD = 0x01;	//Timer 0, mode 1
	TL0 = 0xFD;		// 50mS Delay. FFFFh-4BFDh = B402h = 46082+1 = 46083
	TH0 = 0x4B;		// 46083 X 1.085uS = 50mS
	TR0 = 1;			// Turn ON timer
	while(TF0 == 0);	// wait for TF0 to roll over
	TR0 = 0;			// Turn OFF Timer
	TF0 = 0;			// reset TF0	
}
