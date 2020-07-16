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

void display_string();
void look_uptable(unsigned char display_val);
unsigned char val[4]= "8051";
unsigned char tmpval[4]= "8051";

bit serial_flag;
unsigned char data_temp;
// add lcd.c and varflag.c to run this program.


void initilise();
void transmit_data();
sbit LED = P1^6;
void main()
{
	int i = 0;
	initilise();
	while(1)
	{   
		if(serial_flag == 1)
		{
			tmpval[i] = data_temp;
			i++;
			if(i>3)
			{
				i = 0;
				val[0] = tmpval[0];
				val[1] = tmpval[1];
				val[2] = tmpval[2];
				val[3] = tmpval[3];
			}
			serial_flag = 0;
		}
		display_string();
	}
}

/******************************************************************************

  *  Function        :  initialise().                                                                                 
 																			  	
  *  Purpose         : Initialises ports, port pins of display, flags and enable 
						interrupt for serial port required in the program.
                              
  * Parameters       : None.	
  
  * Globals Used     : None.
                  
  * Return           : None.
                                
*******************************************************************************/

void initilise()
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
	P2 = 0x00;

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

void display_string()
{
	unsigned int x;

	for(x=0;x<4;x++)
	{
		P2=0xFF;
		P1 = ~(1<<x);
		look_uptable(val[x]);
	}
}

void look_uptable(unsigned char display_val)
{
	int i;
	switch(display_val)
	{
		case '0':P2=~0x3F;
			break;
		
		case '1':P2=~0x06;
			break;
		
		case '2':P2=~0x5B;
			break;
		
		case '3':P2=~0x4F;
			break;
		
		case '4':P2=~0x66;
			break;
		
		case '5':P2=~0x6D;
			break;
		
		case '6':P2=~0x7D;
			break;
		
		case '7':P2=~0x07;
			break;
		
		case '8':P2=~0x7F;
			break;
		
		case '9':P2=~0x6F;
			break;
		
		case 'A':
		case 'a':
			P2=~0x77;
			break;
		case 'B':
		case 'b':
			P2=~0x7C;
			break;
		case 'C':
		case 'c':
			P2=~0x39;
			break;
		case 'D':
		case 'd':
			P2=~0x5E;
			break;
		case 'E':
		case 'e':
			P2=~0x79;
			break;
		case 'F':
		case 'f':
			P2=~0x71;
			break;
		case 'G':
		case 'g':
			P2=~0x6F;
			break;
		case 'H':
		case 'h':
			P2=~0x74;
			break;
		case 'I':
		case 'i':
			P2=~0x30;
			break;
		case 'J':
		case 'j':
			P2=~0x0E;
			break;
		case 'K':
		case 'k':
			P2=~0x70;
			break;
		case 'L':
		case 'l':
			P2=~0x38;
			break;
		case 'M':
		case 'm':
			P2=~0x55;
			break;
		case 'N':
		case 'n':
			P2=~0x54;
			break;
		case 'O':
		case 'o':
			P2=~0x5C;
			break;
		case 'P':
		case 'p':
			P2=~0x73;
			break;
		case 'Q':
		case 'q':
			P2=~0x6B;
			break;
		case 'R':
		case 'r':
			P2=~0x50;
			break;
		case 'S':
		case 's':
			P2=~0x6D;
			break;
		case 'T':
		case 't':
			P2=~0x78;
			break;
		case 'U':
		case 'u':
			P2=~0x3E;
			break;
		case 'V':
		case 'v':
			P2=~0x1C;
			break;
		case 'W':
		case 'w':
			P2=~0x6A;
			break;
		case 'X':
		case 'x':
			P2=~0x76;
			break;
		case 'Y':
		case 'y':
			P2=~0x6E;	
			break;
		case 'Z':
		case 'z':
			P2=~0x5B;
			break;
		case '!':
			P2=~0x40;
			break;
		
		default :
			P2=~0x00;	
	}
	for(i=0;i<200;i++);						 
}


