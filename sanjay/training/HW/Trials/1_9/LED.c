#include <reg51.h>

unsigned char lookup(char x);
#define DISP_PORT P0
#define CNTRL_PORT P1

void SelectSegment(unsigned int);
void Delay(void);
unsigned char ARY[] = {"1234"};
void main()
{
	unsigned int n = 0;
	while(1)
	{
		DISP_PORT = 0xFF;
		SelectSegment(n);
		DISP_PORT = lookup(ARY[n]);
		n = (++n) % 4;
		Delay();
	}
}

void Delay(void)
{
	unsigned int i;
	for(i=0;i<20;i++);
//	for(i=0;i<20000;i++);
//	for(i=0;i<20000;i++);
//	for(i=0;i<20000;i++);
}

void SelectSegment(unsigned int x)
{
	CNTRL_PORT = ~(1<<x);
}
	
unsigned char lookup(char x)
{
	unsigned char mDATA;
	switch(x)
	{
		case '0' : mDATA=~0x3f; break;
		case '1' : mDATA=~0x06;	break;
		case '2' : mDATA=~0x5b;	break;
		case '3' : mDATA=~0x4f;	break;
		case '4' : mDATA=~0x66;	break;
		case '5' : mDATA=~0x6d;	break;
		case '6' : mDATA=~0x7d;	break;
		case '7' : mDATA=~0x07;	break;
		case '8' : mDATA=~0x7f;	break;
		case '9' : mDATA=~0x67;	break;
		default : mDATA=0x00;
	}
	return mDATA;
}

	

