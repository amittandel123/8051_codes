#include <reg51.h>
#include "I2C_EEPROM.H"
sbit WP = P2^0;

int xyz  _at_ 0x100;

#define pqr (*((unsigned int xdata *)0x1000))

void initialise(void)
{
	P0 		= 0xFF;
	pqr 	= 0x05;
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

unsigned char CriptData(unsigned char mData)
{
	unsigned char RetVal=0,tmp, i, MASK  = 0xC0;
	for(i=0;i<8;i+=2)
	{
		tmp = mData & MASK;
		tmp >>= (6-i);
 		MASK >>= 2;
		RetVal <<= 2;
		if(tmp==0x00)
			RetVal |= 0x00; 
		else if(tmp==0x01)
			RetVal |= 0x3; 
		else if(tmp==0x02)
			RetVal |= 0x02; 
		else if(tmp==0x03)
			RetVal |= 0x01; 
	}
	return RetVal;
}

extern int test(void);
void main(void)
{
	int xyz = 20, mno = 1;
	unsigned char y,asm,Dev_Add = 0xA0,ReadByte;
	unsigned int add;
	unsigned int No_Of_Bytes_2_Write = 100;
	unsigned char data_2_I2C[]= {18,100,86,120,1,0,0,24,25,26,27,10,20,30,40,50,60,70,80,32,0,0,0,0,0};
	unsigned char PWD[] = {120,86,100,18}; 
	unsigned char key[] = 	{	0xA9,0x00,0x00,0x00,0x0D,0xE2,0x37,0x0C,0x00,0x00,0x00,	//blue
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00, // deleted
								0x2D,0x00,0x00,0x00,0x0E,0x11,0xCE,0x0C,0x00,0x00,0x00,	//blue
//								0x3C,0x00,0x00,0x0D,0xA8,0xB3,0x2E,0x01,0xFF,0x00,0x00,	//red
//								0xCD,0x00,0x00,0x0D,0xAC,0x42,0x34,0x01,0xFF,0x00,0x00,	//red
//								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00, // deleted
//								0xEA,0x00,0x00,0x0D,0xA6,0x66,0x80,0x01,0xFF,0x00,0x00,	//red
							};


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

/*		for(y=0;y<25;y++)
		{	
			I2C_Write(CriptData(data_2_I2C[y]), 0xA0, 0x00, y); // (data,Dev_add,word1,word2)
			wait(2);
		}	
		
		for(y=0;y<33;y++)
		{
			I2C_Write(CriptData(key[y]), 0xA0, 0x00, 0x0019 + y); // (data,Dev_add,word1,word2)
			wait(2);
		}

		while(1);	
*/
	//***************************************************************************
	//*						Reading Data from Memory							*
	//***************************************************************************

	add=0;
	asm=0;
	No_Of_Bytes_2_Write = 20;
	while(No_Of_Bytes_2_Write--)
	{	
		ReadByte = CriptData(I2C_Read(Dev_Add,asm,add));
		Print(ReadByte);
		if(add==255)
		{
			asm++;
		}
		add++;
	}

	PCON |=0x03;
	No_Of_Bytes_2_Write = 200;
	
	add=0;
	asm=4;
	while(No_Of_Bytes_2_Write--)
	{	
		ReadByte = CriptData(I2C_Read(Dev_Add,asm,add));
		Print(ReadByte);
		if(add==255)
		{
			asm++;
		}
		add++;
	}
	while(1);
}
