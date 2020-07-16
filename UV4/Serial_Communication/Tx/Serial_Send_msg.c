// Write an 8051 program to (a) send to PC the message “We Are Ready”, (b) receive any data send
// by PC and put it on LEDs connected to P1, and (c) get data on switches connected to P2 and send 
// it to PC serially. The program should perform part (a) once, but parts (b) and (c) continuously, 
// use 9600 baud rate.

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is the freq provided by UART to Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1

#include<reg51.h>

void Initialize()
{
	TMOD=0x20;
	TH1=0xFD;		//9600 Baud rate
	SCON=0x50;
	TR1=1;			// Start timer
	P2=0xFF;		// P2 as i/p port
}

void Ready(unsigned char ready[])
{
	unsigned char x;
	for(x=0;x<12;x++)
	{
		SBUF=ready[x];
		while(TI==0);
		TI=0;
	}
}

void Transmit(unsigned char mdata[])
{
	while(1)
	{
		SBUF=mdata[];
		while(TI==0);
		TI=0;
	}
}

void Receive()
{
	P2=SBUF;
	while(RI==0);
	RI=0;
	Transmit();
}

void main()
{
	void Initialize();
	void Ready("We are ready");
	void Receive();
	void Transmit();
}