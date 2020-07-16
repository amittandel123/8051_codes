#include<reg51.h>
main()
{
	unsigned int i,j,x;
	P1=0x81;
	while(1)
	{
		for(i=0;i<35000;i++);
		x=0x03;
		for(j=0;j<3;j++)
		{
			P1=(P1>>1)+0x80+x;
			for(i=0;i<35000;i++);
			x=x<<1;
		}
		
		if(P1==0xFF)
		{
			P1=0x00;
			for(i=0;i<35000;i++);
			P1=0x81;
		}
		
		
	}
}