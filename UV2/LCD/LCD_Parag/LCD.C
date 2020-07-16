											//user defined header

	#include<reg51.h>
	#include"LCD.h"
//===========================================================================================================================
											//variable local

													//8 bit data to be displayed on LCD
//============================================================================================================================

											//function body
  sbit EN=P2^2;
  sbit RS=P2^0;
  sbit RW=P2^1;
//ENABLE PIN 0
//RS     PIN 2
												
	void LCD_delay(unsigned int ms)
	{
		unsigned int n;
		unsigned int i;
		unsigned char LCD_DELAY = 100;			  
		for (n=0; n<ms; n++)
			{
				for (i=0; i<LCD_DELAY; i++);
			}     
	}
//---------------------------------------------------------------------------------------------------------------------------

	void LCD_command(unsigned char command)					//Command mode
		{
		    	
			RS=0;
			EN=1;
			P1=command;
			LCD_delay(10);
			EN=0;
			LCD_delay(10);

		}
//---------------------------------------------------------------------------------------------------------------------------

	void LCD_putc(unsigned char ascii)					//Data Mode
		{

				
			RS=1;
			EN=1;
			P1=ascii;
			LCD_delay(10);
			EN=0;
			LCD_delay(10);


		}
//---------------------------------------------------------------------------------------------------------------------------

	void LCD_puts(unsigned char *lcd_string)   
		{															  			
				while (*lcd_string) 				//first row data
					{					       																		   
						LCD_putc(*lcd_string++);
					} 											
		}
//---------------------------------------------------------------------------------------------------------------------------
	
	void LCD_row_col(unsigned char row ,unsigned char col)
		{
			switch(row)
				{
				 	case 1:		LCD_command(0x80+(col-1));
								break;
									
					case 2: 	LCD_command(0xC0+(col-1));
								break;

					default:	break;	

				}		   		 
		}






//---------------------------------------------------------------------------------------------------------------------------

	void LCD_init()
		{	
			RW=0;
			LCD_command(0x01);	// Clear Display												//Clear Display
			LCD_delay(20);														//Let LCD Driver stabilize
//			LCD_command(0x33);													//8 bit mode, 8 bit mode
//			LCD_command(0x33);													//8 bit mode, 8 bit mode
//			LCD_command(0x32);													//8 bit mode, 4 bit mode
			LCD_command(0x38);													//8 bit mode, 2 line mode
			LCD_command(0x0C);													//Display Cursor (Underline and Blink)
			LCD_command(0x03);													//Display and Cursor Home
			LCD_delay(20);														//Display and Cursor Home requres more time
			LCD_command(0x01);													//Clear Display
			LCD_delay(20);														//Clear Display requires more time
		}
//---------------------------------------------------------------------------------------------------------------------------


