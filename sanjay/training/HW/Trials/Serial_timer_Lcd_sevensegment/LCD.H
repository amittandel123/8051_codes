#ifndef __LCD_H__
#define __LCD_H__
#include <reg51.h>
#include <intrins.h>
#include <stdio.h>
	#define LCD_DATA_PORT P0

	extern void writeData_LCD(unsigned char mdata[], unsigned int counter);
	extern void display_data(unsigned char port_data);
	extern void init_LCD(void);
	extern void delay(unsigned int val);
	extern void SendCommand(unsigned char comd);
	extern void ClearDisplay(void);
	extern void writeCommand_LCD(void);
#endif
