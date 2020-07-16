#include<reg51.h>
#include"LCD.H"
  
void main()
{
	LCD_init();
	
	LCD_puts("Happy Diwali");
	
	while(1);	
}


