
#include<reg51.h>
#include<intrins.h>
#include"iButton.h"

#define A 6
#define B 64
#define C 60
#define D 10
#define E 9
#define F 55
#define G 0
#define H 480
#define I 70
#define J 410

#define Port1_Input  (P1=0x00)
#define Port1_Output (P1=0xFF)

sbit iB_Pin = P1^0;

void tickDelay(unsigned int tick)	// Delay of 'tick*1uS'
{
	unsigned int i;
	for(i=0;i<tick;i++)
		_nop_();
}

//-----------------------------------------------------------------------------
// Generate a 1-Wire reset, return 1 if no presence detect was found,
// return 0 otherwise.
// (NOTE: Does not handle alarm presence from DS2404/DS1994)
//

unsigned int Reset_iButton(void)
{
	unsigned int result;
	tickDelay(G);
	Port1_Output; 		//make P1 output port
	P1 = 0x00;
	tickDelay(H);		// 480uS
	P1 = 0x01;
	tickDelay(I);		// 70uS
	Port1_Input; 		//make P1 an input
	result = P1^0x01; 	// Sample for presence pulse from slave
	tickDelay(J); 		// 410uS
	return result;
/*	if(result==1)
		return 0;
	else
		return 1;
*/
}

//-----------------------------------------------------------------------------
// Send a 1-Wire write bit. Provide 10us recovery time.
//
void Write_Bit(unsigned int a)
{
	if (a)
	{
		// Write '1' bit
		Port1_Output;
		P1 = 0;
		tickDelay(A);
		P1 = 1;
		tickDelay(B);
	}
	else
	{
		// Write '0' bit
		Port1_Output;
		P1 = 0;
		tickDelay(C);
		P1 = 1;
		tickDelay(D);
	}
}

//-----------------------------------------------------------------------------
// Read a bit from the 1-Wire bus and return it. Provide 10us recovery time.
//
unsigned int Read_Bit(void)
{
	unsigned int result;
	Port1_Output;
	P1 = 0;
	tickDelay(A);
	P1 = 1;
	tickDelay(E);
	Port1_Input;
	result = P1^0x01; 	// Sample the bit value from the slave
	tickDelay(F);
	return result;
}

//-----------------------------------------------------------------------------
// Write 1-Wire data byte
//
void Write_Byte(unsigned int val)
{
	unsigned int loop;
	for (loop = 0; loop < 8; loop++)	// Loop to write each bit in the byte, LS-bit first
	{
		Write_Bit(val & 0x01);
		val >>= 1;						// shift the data byte for the next bit
	}
}

//-----------------------------------------------------------------------------
// Read 1-Wire data byte and return it
//
unsigned int Read_Byte(void)
{
	unsigned int loop, result=0;
	for(loop = 0; loop < 8; loop++)
	{
		result >>= 1;		// shift the result to get it ready for the next bit
		if(Read_Bit())		// if result is one, then set MSBit
			result |= 0x80;
	}
	return result;
}
//-----------------------------------------------------------------------------
// Write a 1-Wire data byte and return the sampled result.
//
unsigned int Touch_Byte(unsigned int val)
{
	unsigned int loop, result=0;
	for(loop = 0; loop < 8; loop++)
	{
		result >>= 1;		// shift the result to get it ready for the next bit
		if(val & 0x01)		// If sending a '1' then read a bit else write a '0'
		{
			if (Read_Bit())
				result |= 0x80;
		}
		else
			Write_Bit(0);	
		val >>= 1;			// shift the data byte for the next bit
	}
	return result;
}

//-----------------------------------------------------------------------------
// Write a block 1-Wire data bytes and return the sampled result in the same
// buffer.
//
void Write_Block(unsigned char *val, unsigned int data_len)
{
	unsigned int loop;
	for (loop = 0; loop < data_len; loop++)
	{
		val[loop] = Touch_Byte(val[loop]);
	}
}