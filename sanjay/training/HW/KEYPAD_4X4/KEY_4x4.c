#include <intrins.h>
#include<reg51.h>
#include "Keydata.h"
#include "I2CDATA.H"

sbit EN = P3^5;

int prev_pressed_key=0, pressed_key=0;
void scan(x); 

void init_serial()
{
	TR1 = 0;
 	ES = 0;
 	EA = 0;
 	SCON  = 0x40;		        /* SCON: mode 1, 8-bit UART, disable rcvr */
 	TMOD = 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload */
 	TH1   = 253;                /* TH1:  reload value for 9600 baud @ 11.059 MHz */
 	TR1   = 1;                  /* TR1:  timer 1 run */

	EA = 1;
	ES = 1;
	RI = 0;
	TI = 0;
}


void printstr(unsigned char * mstr)
{
	while(*mstr != '\0')
	{
		SBUF = *mstr;
		mstr++;
		while(!TI);
		TI = 0;
	}
}


void display(int num)
{
	unsigned char mstr[8] = "KEY : ";
	if (num<10)
		mstr[6] = num + 0x30;
	else
		mstr[6] = num + 0x41 - 10;

	mstr[7] = '\0';
	printstr(mstr);
}

void initi_key(void)
{
	P1 = 0xF8;
	P2 = 0x00;
	EN=0;
	EA=0;
	P0=0xFF;
}

void scan (int x);
{
	for(rw=0;rw<4;rw++)
			{
				P0 = ROWS[rw] + 0x0F;
				KEY_VAL=P0;
				if((KEY_VAL & 0x0F)!= 0x0F)
				{
					for(cl=0;cl<4;cl++)
					{
						if((KEY_VAL & 0x0F) == COLS[cl])
						{
							pressed_key = ((cl*4)+(rw+1));
							x=pressed_key;
							P0 = 0xFF;
							while(P0 != 0xFF);
							break;
	
	
	
}
	
 /*void main(void)
{
	int rw, cl;
	unsigned char KEY_VAL=0;
	unsigned char ROWS[4] = {0xE0,0xD0,0xB0,0x70};
	unsigned char COLS[4] = {0x0E,0x0D,0x0B,0x07};

	initi_key();
	init_serial();

	while(1)
	{
		for(rw=0;rw<4;rw++)
		{
			P0 = ROWS[rw] + 0x0F;
			KEY_VAL=P0;
			if((KEY_VAL & 0x0F)!= 0x0F)
			{
				for(cl=0;cl<4;cl++)
				{
					if((KEY_VAL & 0x0F) == COLS[cl])
					{
						pressed_key = ((cl*4)+(rw+1));
						
						P0 = 0xFF;
						while(P0 != 0xFF);
						break;
					}
				}
			}
		}
	}
}   */


