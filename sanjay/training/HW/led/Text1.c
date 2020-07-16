#include<reg51.h>

int i;
void main(void)
{
	P0=0xAA;
	while(1)
	{
		for(i=0;i<30000;i++);
		P0=~P0;
	}
}
