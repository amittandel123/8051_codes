// Program to test serial communication of controller with PC using hyper terminal

#include<reg51.h>

// The m/c cycle freq of 8051=11.0529MHz/12=921.6kHz.
// The 8051's serial communication UART cktry divides the m/c cycle freq of 921.6kHz by 32once more
// before it is used by Timer 1 to set the baud rate.
// 921.6kHz/32=28800Hz is the freq provided by UART to Timer1 to set baud rate.
// (a) 28800/3=9600  => 3=FD(hex) is loaded into TH1 
// (b) 28800/12=2400 => 12=F4(hex) is loaded into TH1
// (c) 28800/24=1200 => 24=E8(hex) is loaded into TH1

void ini()     // Initialize Timer 1 for serial communication
{
	TMOD=0x20;  //Timer1, mode 2
	TH1=0XFD; 	// baud rate 9600 bps
	SCON=0x50;
	TR1=1;
}

void receive()  //Function to receive serial data
{
	unsigned char value;
	while(RI==0);
	value=SBUF;
	P1=value;
	RI=0;
}

void transmit()  // Funtion to transmit serial data
{
	P2=P1-32;
	SBUF=P2;
	while(TI==0);
	TI=0;
	SBUF=P1;
	while(TI==0);
	TI=0;
}

void main()
{
	while(1)
	{
		ini();
		receive();
		transmit();
	}
}
