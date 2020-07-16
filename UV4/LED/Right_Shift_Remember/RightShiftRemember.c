#include<reg51.h>
main()
{
	unsigned int i;
	P1=0x80;
	while(1)
	{
		P1=((P1>>1)+128);
		for(i=0;i<35000;i++);
		if(P1==0xFF)
		{
			P1=0x80;
		}
		for(i=0;i<35000;i++);
		
	}
}