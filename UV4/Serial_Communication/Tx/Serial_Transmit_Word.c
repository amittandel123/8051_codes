// Write a program for the 8051 to transfer “I am the BEST” serially at 9600
// baud, 8-bit data, 1 stop bit, do this continuously

#include<reg51.h>

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is the freq provided by UART to Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1

void Initialize()
{
	TMOD=0x20;		// Timer1 mode2 
	TH1=0xFD;		// Baud rate 9600
	SCON=0x50;		// 8-bit, 1 stop, REN enabled
	TR1=1;			// start timer	
}

void Transmit()
{
	while(1)
	{
		unsigned char x,mdata[]=("I am the BEST ");
		for(x=0;x<14;x++)
		{
			SBUF=mdata[x];
			while(TI==0);
			TI=0;
		}	
		
	}
}
/*
void Delay(unsigned char value)
{
	unsigned int u,v;
	for(u=0;u<value;u++)
		for(v=0;v<1000;v++); 
}
*/

void main()
{
	Initialize();
	Transmit();	
}