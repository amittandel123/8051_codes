#include<reg51.h>	   
void main()
{
	while(1)
	{
		unsigned int i,j,k;
		P0=0x01;
		P1=0x01;
		P2=0x01;
		while(1)
		{
			P0=P0<<1;
			P2=P2<<1;
			P1=P1<<1;

			if(P0==0x80)
			{
				for(i=0;i<10000;i++);
				P1=0x01;
			}
			if(P1==0x80)
			{
				for(j=0;j<10000;j++);
				P1=0x01;
			}
			if(P2==0x80)
			{
				for(k=0;k<10000;k++);
				P1=0x01;
			}
		}
	}
}		