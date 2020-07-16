/************************************************************************************************
Program to read the keypad & send the result to serial port.
Serial port configured for 9600baud, 8bit.

************************************************************************************************/

#include<reg51.h>

#define COL P2
#define ROW P1

void MSDelay(unsigned int value);
void SerTX(unsigned char);

unsigned char keypad[4][4]= { 
							  '0','1','2','3',
							  '4','5','6','7',
							  '8','9','A','B',
							  'C','D','E','F'
							};
void main()
{
	unsigned char colloc, rowloc;
	TMOD=0x20;	// Timer 1, mode2
	TH1=-3;		// 9600 baud rate
	SCON=0x50;	// 8 bit, 1 stop bit
	TR1=1;		// Start timer 1

	COL=0xFF;	// P2 as i/p	

	while(1)
	{
		do
		{
			ROW=0x00;				// Ground all rows at once
			colloc=COL;				// Read column
			colloc=colloc & 0x0F;	// mask used bits
		}while(colloc != 0x0F);		// Check until all keys released

		do
		{
			do
			{
				MSDelay(5);		
				colloc=COL;			// See if any key is pressed
				colloc=colloc&0x0F;	// Mask unused bits
			}while(colloc==0x0F);	// Keep checking for keypress

			MSDelay(5);
			colloc=COL;				// Read columns
			colloc &= 0x0F;			// Mask unused bits
		}while(colloc==0x0F);		// Wait for keypress

		while(1)
		{
			ROW=0xFE;				// Ground Row 0
			colloc=COL;				// Read columns
			colloc &= 0x0F;			// Mask unused bits
			if(colloc != 0x0F)		
			{
				rowloc = 0;			// Save row location
				break;				// Exit while loop
			}

			ROW=0xFD;				// Ground Row 1
			colloc=COL;				// Read columns
			colloc &= 0x0F;			// Mask unused bits
			if(colloc != 0x0F)		
			{
				rowloc = 1;			// Save row location
				break;				// Exit while loop
			}

			ROW=0xFB;				// Ground Row 2
			colloc=COL;				// Read columns
			colloc &= 0x0F;			// Mask unused bits
			if(colloc != 0x0F)		
			{
				rowloc = 2;			// Save row location
				break;				// Exit while loop
			}

			ROW=0xF7;				// Ground Row 3
			colloc=COL;				// Read columns
			colloc &= 0x0F;			// Mask unused bits
			rowloc = 3;				// Save row location
			break;					// Exit while loop
			
		}
		if(colloc==0x0E)
			SerTX(keypad[rowloc][0]);
		else if(colloc==0x0D)
			SerTX(keypad[rowloc][1]);
		else if(colloc==0x0B)
			SerTX(keypad[rowloc][2]);
		else
			SerTX(keypad[rowloc][3]);
	}
}  									// End of Main()

void SerTX(unsigned char x)
{
	SBUF=x;				// Put value in buffer
	while(TI==0);		// Wait to transmit last bit
	TI=0;				// Clear busy flag
}

void MSDelay(unsigned int value)
{
	unsigned int u, v;
	for(u=0;u<1275;u++)
		for(v=0;v<value;v++);
}

