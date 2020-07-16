/*  Name     : code.c
 *  Purpose  : Main file for using timer1 of 8051.
 *  Author   : M.Saeed Yasin
 *  Date     : 08-06-12
 *  Revision : None
 */
#include<reg51.h>

// Out Pin
sbit Out = P2^0;		   // Pin P2.0 is named as Out
#define TRUE	1
#define FALSE	0
//Function declarations
void cct_init(void);
void InitTimer1(void);
unsigned int count;
unsigned char TimeUp = FALSE;
int main(void)
{
	cct_init();   	       // Make all ports zero
	InitTimer1();           // Start Timer1
	while(1)                // Rest is done in Timer1 interrupt
	{
		if(TimeUp)
		{
			TimeUp = 0;
			TH1 = 0x00;      // 
			TL1 = 0x00;      // 
			count++;
			if(count == 140)
			{
				count = 0;
				Out = ~Out;  // Toggle Out pin
			}
		}
	}
}

void cct_init(void)
{
	P0 = 0x00;   
	P1 = 0x00;   
	P2 = 0x00;   
	P3 = 0x00;  
}


void InitTimer1(void)
{
	TMOD &= 0x0F;      /* Timer 1 mode 1 with software gate */
	TMOD |= 0x10;      /* GATE0=0; C/T0#=0; M10=0; M00=1; */
	
	TH1 = 0x00;      // 
	TL1 = 0x00;      // 
	
	ET1 = 1;         // Enable Timer1 interrupts
	EA  = 1;         // Global interrupt enable
	TR1 = 1;         // Start Timer 1
}


void Timer1_ISR (void) interrupt 3   // 
{
	TimeUp = TRUE;
	TF1 = 0;     // Clear the interrupt flag
}

