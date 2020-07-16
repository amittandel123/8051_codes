#include<reg51.h>
#include<stdio.h>

sbit LED_A=P0^0;
sbit LED_B=P0^1;
sbit LED_C=P0^2;
sbit LED_D=P0^3;


void initialise();
void display_string();
void look_uptable(unsigned char display_val);
unsigned char val[4]= "8051";

void main()
{
		initialise();
		while(1)
		{
			display_string();
		}
}

void initialise()
{
	P2=0xFF;
	P1=0xFF;
}

void display_string()
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
		
		case 'A':
		case 'a':
			P2=~0x77;
			break;
		case 'B':
		case 'b':
			P2=~0x7C;
			break;
		case 'C':
		case 'c':
			P2=~0x39;
			break;
		case 'D':
		case 'd':
			P2=~0x5E;
			break;
		case 'E':
		case 'e':
			P2=~0x79;
			break;
		case 'F':
		case 'f':
			P2=~0x71;
			break;
		case 'G':
		case 'g':
			P2=~0x6F;
			break;
		case 'H':
		case 'h':
			P2=~0x74;
			break;
		case 'I':
		case 'i':
			P2=~0x30;
			break;
		case 'J':
		case 'j':
			P2=~0x0E;
			break;
		case 'K':
		case 'k':
			P2=~0x70;
			break;
		case 'L':
		case 'l':
			P2=~0x38;
			break;
		case 'M':
		case 'm':
			P2=~0x55;
			break;
		case 'N':
		case 'n':
			P2=~0x54;
			break;
		case 'O':
		case 'o':
			P2=~0x5C;
			break;
		case 'P':
		case 'p':
			P2=~0x73;
			break;
		case 'Q':
		case 'q':
			P2=~0x6B;
			break;
		case 'R':
		case 'r':
			P2=~0x50;
			break;
		case 'S':
		case 's':
			P2=~0x6D;
			break;
		case 'T':
		case 't':
			P2=~0x78;
			break;
		case 'U':
		case 'u':
			P2=~0x3E;
			break;
		case 'V':
		case 'v':
			P2=~0x1C;
			break;
		case 'W':
		case 'w':
			P2=~0x6A;
			break;
		case 'X':
		case 'x':
			P2=~0x76;
			break;
		case 'Y':
		case 'y':
			P2=~0x6E;	
			break;
		case 'Z':
		case 'z':
			P2=~0x5B;
			break;
		case '!':
			P2=~0x40;
			break;
		
		default :
			P2=~0x00;	
	}
	for(i=0;i<200;i++);						 
}

