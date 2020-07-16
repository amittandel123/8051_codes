#include<reg51.h>
#include<intrins.h>
#include<stdio.h>

sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;
sbit led4=P2^3;

main()
{
	unsigned int i;
//	P1=0;
	P2=0;
	while(1)
	{
//		P1=~P1;
		led1 = 1;
		led2 = 0;
		led3 = 1;
		led4 = 0;
		for(i=0;i<50000;i++);
		P2=0;
		for(i=0;i<50000;i++);
	}
}