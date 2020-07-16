#include<reg51.h>
void main()
{
	unsigned char x='4',y='7',BCD_Number;
	x= x & 0x0F;							// masking of lower 4 bits
	y= y & 0x0F;							// masking of higher 4 bits
	x = x<<4;  								// making it higher bits
	BCD_Number = x | y;
	P1 = BCD_Number; 
}