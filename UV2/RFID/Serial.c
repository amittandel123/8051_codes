
// Program for serial communication

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is the freq provided by UART to Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1


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
	unsigned char k,mybyte;
    for(k=0;k<12;k++)
	{
		while(RI==0);	 //wait to receive
		mybyte=SBUF; 	//save value to mybyte
		RI=0;
	}
	return(mybyte);
}
