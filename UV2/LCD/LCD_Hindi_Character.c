
//Program to display hindi alphabets. This program displays a set of five hindi characters at a time due to the size CG Ram.
// To display any other set of five characters use corresponding values from the array.

#include<reg51.h>
#define cmdport P2 
#define dataport P1 
sbit rs = cmdport^0;  //Register select (RS) pin
sbit rw = cmdport^1;  //Read write (RW) pin)
sbit en = cmdport^2;  //Enable (EN) pin

void delay(unsigned int msec)  //Time delay function
{
	int i,j ;
	for(i=0;i<msec;i++)
		for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char item)  //Function to send command to LCD
{
	dataport = item;
	rs=0; 
	rw=0; 
	en=1; 
	delay(1);
	en=0;
}

void lcd_data(unsigned char item)  // Function to send data to LCD
{
	dataport = item;
	rs=1; 
	rw=0; 
	en=1; 
	delay(1);
	en=0;
}

lcd_shape(unsigned char *letter, int j)  // Function to create custom shapes
{
	int i;
	lcd_cmd(j);
	for(i=0;i<8;i++)
	{
		lcd_data(*letter);
	    letter++;
	}
//	return 0;
}

void main()
{
	unsigned char letter[][8]={31,4,14,21,21,13,4,4,
								31,9,11,21,19,9,9,0,
								31,9,9,9,9,25,25,0,
								31,9,17,13,17,17,15,1,
								31,4,12,16,12,2,18,12};
								
								/*0,31,0,14,21,14,0,0,	
							   0,31,0,14,21,14,0,0,
							   0,6,2,14,2,14,10,2,
							   0,0,0,31,5,29,21,5,
							   0,0,0,0,0,0,0,0};
								
	{0,31,2,26,10,6,2,0,
	31,12,16,12,16,22,15,1,31,1,7,21,21,13,0,0,5,29,17,31,5,29,25,9,1,1,29,7,5,29,1,0};*/
	/*{31,4,4,24,16,17,14,0,31,4,4,14,17,17,14,0,
	31,4,12,16,12,2,18,12,31,4,12,16,22,14,0,0,31,21,21,21,21,9,0,0};*/
	/*{31,1,15,9,9,1,0,0,15,13,5,9,15,1,0,0,31,2,14,16,19,14,2,0,3,25,17,12,17,17,15,1,
	31,1,1,31,9,1,1,0,31,9,9,9,15,1,1,1};*//*{31,9,9,9,7,1,1,1,31,20,20,31,5,5,5,0,31,2,30,26,22,30,2,2,0,26,26,10,30,26,2,2,31,10,10,10,30,26,2,2,};*/
	/*{31,29,5,5,9,31,1,1,31,12,2,2,28,8,4,3,31,1,1,27,21,21,17,9,31,2,30,18,18,30,2,2,29,21,29,5,25,25,9,5};{17,25,21,19,31,1,1,1,31,25,
	5,5,5,15,17,9,31,4,28,16,28,16,16,30,9,21,13,23,17,21,13,5,2,18,10,6,6,10,18,2};*///assigning values of alphabets to an array
	
	unsigned char  i=0;
	char a = 0x80;  // Set cursor to first position of first line
	int z;
	lcd_cmd(0x38);	// 2 Lines & 5X7 matrix
	lcd_cmd(0x0e);	// Display ON, Cursor Blinking
	delay(50);
	lcd_cmd(0x01);  //Clear LCD screen
	delay(50);
	while(1)
	{
		  z=64;
		  a=0x80; 
		  for(i=0;i<5;i++)
		  {
			   lcd_shape(&letter[i][0],z);  //Send different alphabet values to shape func to custom different alphabets
			   delay(50);
			   lcd_cmd(a);  //Move cusrsor to ath position 
			   lcd_data(i);  //Display the alphabet on LCD
			   delay(50);
//			   a=a+2;  
			   a++;
			   z=z+8;  // Change the address register where values are to be stored(e.g. 64,72,80.......)
		  }
		  lcd_cmd(0x01);  //Clear LCD screen
	}
}