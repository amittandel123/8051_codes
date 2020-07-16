#include<reg51.h>
#include<stdio.h>
#include "SEVENSEG.H"

#define DATA_PORT P2
#define CNTRL_PORT P1

unsigned char val[4]= "8051";

void init_sevenSeg(void)
{
	DATA_PORT = 0xFF;
	CNTRL_PORT= 0xFF;
}

void display_string(void)
{
	unsigned int x;

	for(x=0;x<4;x++)
	{
		DATA_PORT=0xFF;
		P1 = ~(1<<x);
		look_uptable(val[x]);
	}
}

void look_uptable(unsigned char display_val)
{
	unsigned int i;
	switch(display_val)
	{
		case '0':DATA_PORT=~0x3F;
			break;
		
		case '1':DATA_PORT=~0x06;
			break;
		
		case '2':DATA_PORT=~0x5B;
			break;
		
		case '3':DATA_PORT=~0x4F;
			break;
		
		case '4':DATA_PORT=~0x66;
			break;
		
		case '5':DATA_PORT=~0x6D;
			break;
		
		case '6':DATA_PORT=~0x7D;
			break;
		
		case '7':DATA_PORT=~0x07;
			break;
		
		case '8':DATA_PORT=~0x7F;
			break;
		
		case '9':DATA_PORT=~0x6F;
			break;
		
		case 'A':
		case 'a':
			DATA_PORT=~0x77;
			break;
		case 'B':
		case 'b':
			DATA_PORT=~0x7C;
			break;
		case 'C':
		case 'c':
			DATA_PORT=~0x39;
			break;
		case 'D':
		case 'd':
			DATA_PORT=~0x5E;
			break;
		case 'E':
		case 'e':
			DATA_PORT=~0x79;
			break;
		case 'F':
		case 'f':
			DATA_PORT=~0x71;
			break;
		case 'G':
		case 'g':
			DATA_PORT=~0x6F;
			break;
		case 'H':
		case 'h':
			DATA_PORT=~0x74;
			break;
		case 'I':
		case 'i':
			DATA_PORT=~0x30;
			break;
		case 'J':
		case 'j':
			DATA_PORT=~0x0E;
			break;
		case 'K':
		case 'k':
			DATA_PORT=~0x70;
			break;
		case 'L':
		case 'l':
			DATA_PORT=~0x38;
			break;
		case 'M':
		case 'm':
			DATA_PORT=~0x55;
			break;
		case 'N':
		case 'n':
			DATA_PORT=~0x54;
			break;
		case 'O':
		case 'o':
			DATA_PORT=~0x5C;
			break;
		case 'P':
		case 'p':
			DATA_PORT=~0x73;
			break;
		case 'Q':
		case 'q':
			DATA_PORT=~0x6B;
			break;
		case 'R':
		case 'r':
			DATA_PORT=~0x50;
			break;
		case 'S':
		case 's':
			DATA_PORT=~0x6D;
			break;
		case 'T':
		case 't':
			DATA_PORT=~0x78;
			break;
		case 'U':
		case 'u':
			DATA_PORT=~0x3E;
			break;
		case 'V':
		case 'v':
			DATA_PORT=~0x1C;
			break;
		case 'W':
		case 'w':
			DATA_PORT=~0x6A;
			break;
		case 'X':
		case 'x':
			DATA_PORT=~0x76;
			break;
		case 'Y':
		case 'y':
			DATA_PORT=~0x6E;	
			break;
		case 'Z':
		case 'z':
			DATA_PORT=~0x5B;
			break;
		case '!':
			DATA_PORT=~0x40;
			break;
		
		default :
			DATA_PORT=~0x00;	
	}
	for(i=0;i<200;i++);
}

