/*
Program the 8051 in C to receive bytes of data serially and put them
in P1. Set the baud rate at 4800, 8-bit data, and 1 stop bit.
*/

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is tothe freq provided by UART  Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1

#include <reg51.h>
void main(void)
{
	unsigned char mybyte;
	TMOD=0x20;	 	//use Timer 1, mode 2
	TH1=0xFA; 		//4800 baud rate
	SCON=0x50;
	TR1=1; 			//start timer
	while (1) 
	{ 
		while(RI==0);	 //wait to receive
		mybyte=SBUF; 	//save value
		P1=mybyte; 		//write value to port
		RI=0;
	}
}



