#include <stdio.h>
#include <reg51.h>

void display();
void look_uptable(unsigned char display_val);
unsigned char val[4] = {"0000"};

void initilise();
unsigned int tick_count, timer0_flag,tick_count1,timer1_flag,sec,sec1;

void main()
{
	unsigned int i,j;
	initilise();
	while(1)
	{
		display();
		if(timer0_flag == 1)
		{
			timer0_flag = 0;

		 	sec++;
			if(sec>9999)
				sec = 0;
			sec1 = sec;
			for(j=0;j<4;j++)
			{
				i = sec1%10;
				sec1 /= 10;
				val[3-j] = i + 0x30;
			}
		}
	}
}

void initilise()
{
	
	TMOD = 0x11; 		 //mode 1 is selected for timer0 and timer1

				 // 1ms delay
	TH0 = 0xFC;  		 //as per 11.059MHZ clock
	TL0 = 0x2B;
	timer0_flag = 0;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	tick_count = tick_count1 =sec = sec1= 0;
}

void timer0(void) interrupt 1
{
	TR0 = 0;
	TH0 = 0xFC;
	TL0 = 0x2B;
//	TR0 = 1;

	tick_count++;

	if(tick_count == 700)   	// for creating 1sec delay, call 1ms 
	{
		timer0_flag = 1; 	// delay approx. 900 times.
		tick_count = 0;
	}
	TR0 = 1;
}

void display()
{
	unsigned int x;

	for(x=0;x<4;x++)
	{
		P2=0xFF;
		P1 = ~(1<<x);
		look_uptable(val[x]);
	}
}

void look_uptable(unsigned char display_val)
{
	int i;
	switch(display_val)
	{
		case '0':P2=~0x3F;
			break;
		
		case '1':P2=~0x06;
			break;
		
		case '2':P2=~0x5B;
			break;
		
		case '3':P2=~0x4F;
			break;
		
		case '4':P2=~0x66;
			break;
		
		case '5':P2=~0x6D;
			break;
		
		case '6':P2=~0x7D;
			break;
		
		case '7':P2=~0x07;
			break;
		
		case '8':P2=~0x7F;
			break;
		
		case '9':P2=~0x6F;
			break;
		
		default :
			P2=~0x00;	
	}

	for(i=0;i<200;i++)
	{
		i=i;
	}					 
}


