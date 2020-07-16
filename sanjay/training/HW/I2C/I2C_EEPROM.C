#include <reg51.h>
#include "I2C_EEPROM.H"
#include "Keydata.h"
#include "I2CDATA.H"
sbit WP = P2^0;

void initialise(void)
{
	P0 		= 0xFF;
	P1 		= 0xFF;
	WP 		= 0x00;
	SCON	= 0x50;
	TMOD	= 0x20;
	TI		= 0x01;
	TR1		= 0x01;
	TH1		= 0xFD;
	EA		= 0x00;
}

void Print(unsigned char mData)
{
	TI		= 0x00;
	SBUF 	= mData;
	while	(!TI);
	TI 		= 0x00;
}

/* void main(void)
{
	int xyz = 20, mno = 1;
	unsigned char y,asm,Dev_Add = 0xA0,ReadByte;
	unsigned int add;
	unsigned int No_Of_Bytes_2_Write = 100;
//	unsigned char data_2_I2C[]= {"Amit Tandel Tarapur M"};
	unsigned char PWD[] = {120,86,100,18}; 


	//	xyz=test();
	initialise();
	I2C_initialise();	
	y=mno;
	mno = xyz+10;
	{
		extern int xyz;
		mno = xyz;
	}
	//***************************************************************************
	//*						Writing Data into Memory							*
	//***************************************************************************

/*	for(y=0;y<21;y++)
	{	
		I2C_Write(data_2_I2C[y], 0xA0, 0x00, y); // (data,Dev_add,word1,word2)
		wait(2);
	}	
*/	//***************************************************************************
	//*						Reading Data from Memory							*
	//***************************************************************************
//	while(1);

/*	add=0;
	asm=0;
	No_Of_Bytes_2_Write = 21;
	while(No_Of_Bytes_2_Write--)
	{	
		ReadByte = I2C_Read(Dev_Add,asm,add);
		Print(ReadByte);
		if(add==255)
		{
			asm++;
		}
		add++;
	}

	while(1);
}
*/
