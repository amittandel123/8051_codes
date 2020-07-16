#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
unsigned int serial_flag;
unsigned char data_temp;
// add lcd.c and varflag.c to run this program.


extern void SendCommand(unsigned char comd);
extern void writeData_LCD(unsigned char mdata[], unsigned int counter);
extern void delay(unsigned int val);

void initilise_serial();
void transmit_N_Disp_data();

void initilise_serial()
{
	TR1 = 0;
 	ES = 0;
 	EA = 0;
 	SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr */
 	TMOD = 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload */
 	TH1   = 253;                /* TH1:  reload value for 9600 baud @ 11.059 MHz */
 	TR1   = 1;                  /* TR1:  timer 1 run */

	EA = 1;
	ES = 1;
	RI = 0;
	TI = 0;
	serial_flag = 0;

}

/******************************************************************************

  *  Function        : serial_intr(void) interrupt 4                                                                              
 																			  	
  *  Purpose         : ISR routine for serial interrupt.
                              
  * Parameters       : None.	
  
  * Globals Used     : None.
                  
  * Return           : None.
                                
*******************************************************************************/


void serial_intr(void) interrupt 4
{
	if(RI)
	{
		RI = 0;
		serial_flag = 1;
		data_temp = SBUF;
	}
	else if (TI)
	{
		TI = 0;
	}
}

/******************************************************************************

  *  Function        : transmit_data()                                                                            
 																			  	
  *  Purpose         : Send message "You have pressed " on terminal first through
					   serial port and then send the data available in temp. 
					   variable thr. serial port Next time data will be displayed
					   on next line.
                              
  * Parameters       : None.	
  
  * Globals Used     : None.
                  
  * Return           : None.
                                
*******************************************************************************/

void printstr(unsigned char * mstr)
{
	unsigned char *tmp;
	unsigned int i,j;	
	tmp = mstr;

	while(*mstr != '\0')
	{
		SBUF = *mstr;
		mstr++;
		while(!TI);
		TI = 0;
	}

	for(i=0;i<10;i++)
	{
		SendCommand(0xC0);// select line 1 on LCD
		delay(5);

		writeData_LCD(tmp,9-i);
		for(j=0;j<1;j++)
			writeData_LCD(" ",1);
		tmp++;
		delay(5);
		for(j=0;j<10000;j++);
		for(j=0;j<10000;j++);
		for(j=0;j<10000;j++);
		for(j=0;j<10000;j++);
	}
}

void transmit_N_Disp_data()
{
	char name[10] = "Kaay re ";

	ES = 0;
	TI=0;

	name[8] = data_temp;
	name[9] = 0;
	printstr(name);

	SBUF = 0x0D;				// clear display of terminal.
	while(!TI);
	TI = 0;

	SBUF = 0x0A;				// Puts cursor of terminal on next line.
	while(!TI);
	TI = 0;
	ES = 1;
}