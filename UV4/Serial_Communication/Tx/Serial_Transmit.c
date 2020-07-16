// Program to serially transmit letter 'A' though serial port

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
	TMOD=0x20;		// Timer 1, Mode 2(8bit auto reload)
	TH1=0xF4;		// 2400 Baud rate
	SCON=0x50;		// Serial Mode 1, 8-bit data, 1 stop bit, 1 start bit
	TR1=1;			// Start timer
}
void Transmit()
{
	while(1)
	{
		SBUF='G';		// Send letter A into buffer
		while(TI==0);	// Monitor Transmission of previous bit
		TI=0;
	}
}
void main()
{
	Initialize();
	Transmit();

}