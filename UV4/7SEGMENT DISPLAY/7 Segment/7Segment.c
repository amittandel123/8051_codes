
#include<reg51.h>

	sbit a=P1^0;
	sbit b=P1^1;
	sbit c=P1^2;
	sbit d=P1^3;
	sbit e=P1^4;
	sbit f=P1^5;
	sbit g=P1^6;
	sbit h=P1^7;

void main()
{
	unsigned int x=0,i;
	P2=0xFE;
	while(1)
	{
//		P2 = 0xF0;
//		P0 = 0xFF;
//		x=12;
		switch(x)
		{
			case 0:
			a=1; b=1; c=1; d=1; e=1; f=1; g=0; h=0;
			break;
	
			case 1: 
			a=0; b=1; c=1; d=0; e=0; f=0; g=0; h=0;
			break;
	
			case 2: 
			a=1; b=1; c=0; d=1; e=1; f=0; g=1; h=0;
			break;
	
			case 3: 
			a=1; b=1; c=1; d=1; e=0; f=0; g=1; h=0;
			break;
	
			case 4: 
	   		a=0; b=1; c=1; d=0; e=0; f=1; g=1; h=0;
			break;
	
			case 5: 
			a=1; b=0; c=1; d=1; e=0; f=1; g=1; h=0;
			break;
	
			case 6: 
			a=1; b=0; c=1; d=1; e=1; f=1; g=1; h=0;
			break;
			
			case 7: 
			a=1; b=1; c=1; d=0; e=0; f=0; g=0; h=0;
			break;
	
			case 8: 
			a=1; b=1; c=1; d=1; e=1; f=1; g=1; h=0;
			break;
	
			case 9:
			a=1; b=1; c=1; d=1; e=0; f=1; g=1; h=0;
			break;
		}
		for(i=0;i<40000;i++);
		x++;
		if(x==10)
		{
			x=0;
		} 	
//		P2 = 0xFF;
	}
}