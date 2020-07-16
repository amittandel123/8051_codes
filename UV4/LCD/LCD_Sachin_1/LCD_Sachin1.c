#include <reg51.h>
#include <stdio.h>
#include <intrins.h>

sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
sbit MOVE_LEFT = P2^3;
sbit MOVE_RGHT = P2^4;
bit LEFT=1;
bit RIGHT=0;
int cursor_count=1;
int update_LED;

sbit left = P0^5;
sbit right = P0^6;
void display_data(unsigned char port_data);
void ClearDisplay(void);
void SendCommand(unsigned char comd);
void delay(unsigned int val);
void move_cursor(int);

void initialise(void)
{
	P1=0xFF;
	P2=0xFF;
	P3=0xFF;
	EN=0;
	RS=0;
	update_LED=0;	
}

void writeCommand_LCD(void)
{
	delay(5);
	SendCommand(0x38);

	delay(5);
	SendCommand(0x38);

	delay(5);
	SendCommand(0x38);

	delay(5);
	SendCommand(0x0C);

	delay(5);
	ClearDisplay();
	delay(5);
}

void SendCommand(unsigned char comd)
{
	RS=0;
	RW=0;  			// to select Command Register
	P0=comd;		// put command on PORT2
	EN=1;			// to Enable the LCD processor
	_nop_();
	EN=0;			// to disable the LCD processor
}


void ClearDisplay(void)
{
	SendCommand(0x01);	// clear display
	delay(15);

	SendCommand(0x02);	//send cursor to home location
	delay(15);
}

void writeData_LCD(unsigned char mdata[], unsigned int counter)
{
	unsigned int cnt;
	if(update_LED == 0)
	{
		for(cnt=0;cnt < counter;cnt++)
		{
			display_data(mdata[cnt]);
			delay(5);
		}
	}
	update_LED=1;
}

void display_data(unsigned char port_data)
{
	RS=1;			// to select DDRAM Register i.e. data register
	P0=port_data;	// put data on port
	EN=1;			// to Enable the LCD processor
	_nop_();
	EN=0;			// to disable the LCD processor
}

void delay(unsigned int val)
{
	unsigned int u,v;
	for(u=0;u<=val;u++)
		for(v=0;v<=100;v++);
}

void main(void)
{
	
	P0=0xFF;
	initialise();
	writeCommand_LCD();
   
	SendCommand(0x80);	// select line 1 on LCD
	delay(10);

	update_LED=0;
	writeData_LCD("BHARGAVI",8);
	
		
	SendCommand(0xC0);	// select line 2 on LCD
	delay(10);
	
	update_LED=0;
	writeData_LCD("KULKARNI",8);  

	SendCommand(0x02);	//send cursor to home location
	delay(5);
	
	SendCommand(0x0E); 	// cursor display
	delay(5);
	
	update_LED=1;
	while(1)
	{
		if(MOVE_LEFT==0x00)
 		{
			move_cursor(LEFT);
		}
		else if(MOVE_RGHT==0x00)
		{
			move_cursor(RIGHT);		
		}
	}
}

void move_cursor(int a)
{
		if(a==RIGHT)
		{
			cursor_count++;
			if(cursor_count!=41 && cursor_count!=1)
			{
				SendCommand(0x30); 	//Right shift
				delay(5);

				SendCommand(0x1C); 	//Right shift
				delay(500);
			}
			
			else if(cursor_count==17)
			{
				SendCommand(0xC0); 
				delay(500);
				cursor_count=41;
			}

			else if(cursor_count==57)
			{
				SendCommand(0x80); 
				delay(500);
				cursor_count=1;
			}
		}
		
		else if(a==LEFT)
		{
			cursor_count--;
			if(cursor_count!=16 && cursor_count!=56)
			{
				SendCommand(0x18); 	//Left shift
				delay(500);
			}

			else if(cursor_count==40)
			{
				SendCommand(0x8F); 
				delay(500);
				cursor_count=16;
			}
		
			else if(cursor_count==0)
			{
				SendCommand(0xCF); 
				delay(500);
				cursor_count=56;
			}
		}
}