
// Program for serial communication

#include <reg51.h>
void Serial_Init()
{
	TMOD=0x20;	 	//use Timer 1, mode 2
	TH1=0xFA; 		//4800 baud rate
	SCON=0x50;
	TR1=1; 			//start timer
} 

unsigned char Serial_Receive()
	{ 
		unsigned char mybyte;
		while(RI==0);	 //wait to receive
		mybyte=SBUF; 	//save value to mybyte
		RI=0;
		return(mybyte);
	}
