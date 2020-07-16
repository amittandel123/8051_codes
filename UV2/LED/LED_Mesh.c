

#include<reg51.h>

void main(void)
{
	unsigned int i;

	P1=0x02;
	P2=0xFF;

	while(1);
	{
		P1=0;
		P2=1;
		for(i=0;i<50000;i++);
		P1=1;
		P2=0;
		for(i=0;i<50000;i++);
	}
		
}
