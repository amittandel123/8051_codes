#include<reg51.h>
main()
{
	unsigned int i;
	P1=0x01;
	while(1)
	{
		P1=((P1<<1)+1);
		for(i=0;i<40000;i++);
		if(P1==0xFF)
		{
			
			P1=0x00;
		}
		
		
	}
}