/*
The Interface of ADC0804 with micro-controller 8051
10:15:00 PM  code, data communication, example program, in c51, keil, PC interface, rs232, The Interface of ADC0804 with 
microcontroller 8051  2 comments

This is example program written keil c51 for the interface of adc0804 single channel adc (analog to digital converter) 
with micro-controller at89s51, which is very much equivalent to at89c51. the only difference is their way of code burning 
in ROM. Any microcontroller of 8051 family can be used here for example, at89c51, at89c52, at89c2051 or from intel also.
The analog voltages are converted by using ADC0804 and then their digital equivalent ranging from 0 to 255 is read out at 
P0 of microcontroller. This digital value of analog voltage (0v to 5v) is then send to computer (PC) through serial 
communication through RS232 with standard null modem protocol on 9600 baud rate. The scanning of port for new value and 
then sending of data to PC is repeated 6 times in a sec.
the example source code in c for microcontroller is as below.
*/

#include<reg51.h>
#include<intrins.h>

unsigned char sec1,sec,adc;
void rdadc0804(void);
void send(void);
sbit en = P3^5;
sbit wt = P3^6;
sbit rd = P3^7;

void send(void);
void rdadc0804(void);
void timer(void);
void serial(void);

void timer(void) interrupt 1
{
	TH0 = 0xdc;
	sec1++;
	if(sec1>=10)
	{
		rdadc();
		setout();
		sec1=0;
	}
}

void main()
{
	EA = 1;
	ET0 = 1;
	SCON = 0x52;
	TMOD = 0x21;
	TH1 = 0xfd; // 9600 baud rate
	TR1 = 1;
	TR0 = 1;
	ES = 1;
	wt = 1;
	rd = 1;
	en = 1;
	adc = 0;
	P0 = 0x00;
	while(1)
	{
		;
	}
}

void send(void)
{
	TI = 0;
	SBUF = adc;
	while(!TI){;}
}

void rdadc0804(void)
{
	wt = 0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	rd = 0;
	_nop_();
	_nop_();
	adc = P1;
	_nop_();
	rd = 1;
	_nop_();
	wt = 1;
}

void serial(void) interrupt 4
{
	if(RI) 
		RI=0;
} 