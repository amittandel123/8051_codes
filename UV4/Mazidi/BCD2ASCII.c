#include<reg51.h>
void main()
{
	unsigned char x,y;
	unsigned char number = 0x33;
	x= number & 0xFF;				// masking of lower 4 bits
	y= number & 0xF0;				// masking of higher 4 bits
	P1 = x | 0x30;
	y = y>>4;
	P2 = y | 0x30; 
}