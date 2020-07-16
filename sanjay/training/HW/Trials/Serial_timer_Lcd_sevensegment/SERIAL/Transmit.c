/******************************************************************************

  *  Analyse            : Receive data through serial port and display that received data
						  on terminal.                                                                    
 																			  	
  *  Purpose            : In this program, the serial interrupt is enabled to receive 
						  data through serial port. So whenever the data is transmitted 
						  through serial port,which goes its SBUF generates serial 
						  interrupt and enters into its ISR routine, where data came 
						  in SBUF will be stored in temp. variable and serial_flag
							will be set. 
	
						  The main function checking this flag contineously and while 
						  this flag is set, it reset the above flag and executes the
						  function transmit_data in 	which data will be received in 
						  SBUF thr. Serial port to display it on terminal with message 
						  " You have pressed --".

*/
#include<reg51.h>
#include <stdio.h>
#include <intrins.h>

//#include "lcd.h"
//#include "port.h"
//#include "varflag.h"

unsigned char data_temp,  serial_flag;
// add lcd.c and varflag.c to run this program.


void initilise();
 void transmit_data();
//sbit LED = P1^6;


/******************************************************************************

  *  Function        :  initialise().                                                                                 
 																			  	
  *  Purpose         : Initialises ports, port pins of display, flags and enable 
						interrupt for serial port required in the program.
                              
  * Parameters       : None.	
  
  * Globals Used     : None.
                  
  * Return           : None.
                                
*******************************************************************************/

void initilise_rs232()
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
	while(*mstr != '\0')
	{
		SBUF = *mstr;
		mstr++;
		while(!TI);
		TI = 0;
	}
}

void transmit_data()
{
	char name[] = "You have pressed ";

	ES = 0;
	TI=0;
	
//	LED = 0;
	printstr(name);

	SBUF = data_temp;
	while(!TI);
	TI = 0;

	SBUF = 0x0D;				// clear display of terminal.
	while(!TI);
	TI = 0;

	SBUF = 0x0A;				// Puts cursor of terminal on next line.
	while(!TI);
	TI = 0;
	ES = 1;
//	LED = 1;
}