#include<reg51.h>
void main()
{
	unsigned int i;
	P1=0x01;
	while(1)
	{
		P1=P1<<1;
		if(P1==0x80)
		{
			for(i=0;i<35000;i++);
			P1=0x01;
		}
		
	}
}