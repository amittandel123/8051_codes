#include<reg51.h>
sbit en=P2^0;
void main()
{
	unsigned char msg[] = "Happy Diwali";
	unsigned char i;
	for(i=0;i<12;i++)
	{
		P1= msg[i];
		en=1;
		en=0;
	}
}