#ifndef __TRANSMIT_H__
#define __TRANSMIT_H__
	extern void transmit_data();
	extern void printstr(unsigned char * mstr);
	extern void initilise_rs232();	
	extern 	unsigned char data_temp,  serial_flag;
	extern void print(unsigned char ch);
#endif
