/* 
Assume that a switch is connected to pin P2.0. Write a program to
monitor the switch and perform the following:
(a) If SW = 0, send the message “Hello” to the Serial #0 port
(b) If SW = 1, send the message “Goodbye” to the Serial #1 port.
*/

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is the freq provided by UART to Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1

 
#include<reg51.h>
sbit SW=P2^0;
void Init()
{
	P2=0xFF;		//P2 as i/p port
	TMOD=0x20;		// Timer1 mode2
	TH1=0xFD;		// 9600 Baud rate
	SCON=0x50;
	TR1=1;			// Start timer

}

void Transmit()
{
	unsigned char x;
	if(SW==0)
	{
		unsigned char mdata[]=("Hello");
		for(x=0;x<5;x++)
		{
			SBUF=mdata[x];
			while(TI==0);
			TI=0;		
		}
	}
	else
	{
		unsigned char mdata[]=("Goodbye");
		for(x=0;x<7;x++)
		{
			SBUF=mdata[x];
			while(TI==0);
			TI=0;		
		}
	}
	
}

void main()
{
	Init();
	Transmit();
}