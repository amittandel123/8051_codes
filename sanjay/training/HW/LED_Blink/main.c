#include<reg51.h>

void main(void)
{
	unsigned int i;
	unsigned char j = 0xFE;
	
	while(1)
	{
		P0 = j;
		j <<=1;
		for(i=0;i<30000;i++);
		if(j == 0x00)
		{
			for(i=0;i<30000;i++);
			j = 0xFE;
		}
	}
}
