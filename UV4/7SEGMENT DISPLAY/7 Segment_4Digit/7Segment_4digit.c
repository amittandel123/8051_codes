// Program to make a bidirectional visitor counter using IR sensor

#include <reg51.h>
#define msec 1
unsigned int num=0;
sbit dig_ctrl_4=P1^3;  //declare the control pins of seven segments 
sbit dig_ctrl_3=P1^2; 
sbit dig_ctrl_2=P1^1; 
sbit dig_ctrl_1=P1^0; 
unsigned int digi_val[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0xF8,0x00,0x10};
unsigned int dig_1,dig_2,dig_3,dig_4,test=0; 
unsigned char dig_disp=0; 
sbit up=P3^5;  //up pin to make counter count up 
sbit down=P3^6;  //down pin to make counter count down

void init()  // to initialize the output pins and Timer0 
{ 
	up=down=1;
	dig_ctrl_4 = 0;
	dig_ctrl_3 = 0;
	dig_ctrl_2 = 0;
	dig_ctrl_1 = 0;
	TMOD=0x01;
	TL0=0xf6;
	TH0=0xFf;
	IE=0x82;
	TR0=1;
}

void delay()  //To provide a small time delay 
{ 
	TMOD=0x01;
    TL0=0x36;
    TH0=0xF6;
    TR0=1;
    while(TF0==0);
    TR0=0;
    TF0=0;
}

void display() interrupt 1  // Function to display the digits on seven segment.
{
	TL0=0x36;
    TH0=0xf6;
	P2=0xFF;
	dig_ctrl_1 = dig_ctrl_3 = dig_ctrl_2 = dig_ctrl_4 = 0; 
	dig_disp++; 
	dig_disp=dig_disp%4;
	
	switch(dig_disp)
	{
	  	  case 0:
		  P2= digi_val[dig_1];
		  dig_ctrl_1 = 1;
		  break;
		
		  case 1:
		  P2= digi_val[dig_2];
		  dig_ctrl_2 = 1;
		  break;
		
		  case 2:
		  P2= digi_val[dig_3];
		  dig_ctrl_3 = 1;
		  break;
		
		  case 3:
		  P2= digi_val[dig_4];
		  dig_ctrl_4 = 1;
		  break;
	}
}

void main()
{
   init();
   while(1)
   {
  		if(up==0&&down==1)  //check if up pin is pressed
     	{
  			test++;
     		num=test;
     		dig_4=num%10;
  			num=num/10;
     		dig_3=num%10;
 			num=num/10;
    		dig_2=num%10;
  			dig_1=num/10;
  			if(test==9999)
  				test=0;
  		}
    	
		if(up==1&&down==0)  //check if down pin is pressed
    	{
  			test--;
		    num=test;
		    dig_4=num%10;
		  	num=num/10;
		    dig_3=num%10;
		  	num=num/10;
		    dig_2=num%10;
		  	dig_1=num/10;
		  	if(test==0)
		  		test=9999;
  		}
	}

}
