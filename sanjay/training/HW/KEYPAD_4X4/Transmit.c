#include<reg51.h>
#include <stdio.h>
#include <intrins.h>

bit serial_flag;
unsigned char data_temp;
// add lcd.c and varflag.c to run this program.


void initilise();
void transmit_data();

void initilise()
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
	serial_flag = 0;

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
