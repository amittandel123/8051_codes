
											//	user defined header																		

//	#include"I2C.h"		
//============================================================================================================================

											//	macro

	#define LCD_clear() LCD_command(0x1)									//Clear display LCD
	#define LCD_origin() LCD_command(0x2)									//Set to origin LCD
	#define LCD_row1() LCD_command(0x80)									//Begin at Line 1
	#define LCD_row2() LCD_command(0xC0)  									//Begin at Line 2
//===========================================================================================================================	

										//	function declaration

	void LCD_init();														//function to initialize LCD
	void LCD_puts(unsigned char * );										//Function to display a string on LCD														//Function to initialize the LCD module
	void LCD_putc(unsigned char );											//Function to display a	character on LCD (4 bit mode data format) 
	void LCD_row_col(unsigned char  ,unsigned char );				//Function to fix cursor position
	void LCD_delay(unsigned int );											//Function for delay in ms
	void LCD_enable_command();									   			//4 bit mode command sending
	void LCD_enable_data();													//4 bit mode data sending
	void LCD_command(unsigned char );										//4 bit mode command format 
//============================================================================================================================

	extern unsigned char LCD_DATA;	