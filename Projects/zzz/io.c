#pragma SMALL DB OE

/*-------------------------------------------------------------------------*/

#include <reg51.h>
#include "io.h"
          
/*-------------------------------------------------------------------------*/

sfr DATA_BUS = 0xa0;
sbit RS = 0xb0;
sbit E  = 0xb1;

/*-------------------------------------------------------------------------*/

static void EnableLCD(int t) {

    unsigned char i;

    E = 1; for(i=0; i<t; i++) i = i;
    E = 0; for(i=0; i<t; i++) i = i;
}

/*-------------------------------------------------------------------------*/

void InitIO(void) {

    RS=0;                                        
    DATA_BUS=0x38; EnableLCD(255);           
    DATA_BUS=0x38; EnableLCD(255);               
    DATA_BUS=0x38; EnableLCD(255);               
    DATA_BUS=0x01; EnableLCD(255);               
    DATA_BUS=0x0d; EnableLCD(255);               
    DATA_BUS=0x06; EnableLCD(255);               
    RS = 1;					 
}

/*-------------------------------------------------------------------------*/

void ClearScreen(void) {

    RS=0;                                        
    DATA_BUS=0x01; EnableLCD(255);               
    RS = 1;					 
}

/*-------------------------------------------------------------------------*/

void GotoXY(unsigned char r, unsigned char c) {

    RS=0;                                        
    DATA_BUS=0x02; EnableLCD(255);               
    for(r=r*40+c, c=0; c<r; c++)
        DATA_BUS = 0x14, EnableLCD(45);
    RS=1;
}

/*-------------------------------------------------------------------------*/

void PutChar(char c) {

    DATA_BUS = c; EnableLCD(45);
}

/*-------------------------------------------------------------------------*/

void PrintString(const char* s) {

    while( *s ) DATA_BUS = *(s++), EnableLCD(45);
}

