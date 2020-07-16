#pragma SMALL DB OE
#include 
#include "io.h"

/* P0, P1, P2 and P3 are predefined port names and are bit addressable */

sbit reset = P0^4; /* bit 4 of Port 0 */
sbit up_down = P0^5;
sbit load = P0^6;
sbit Start_LCD = P0^7; /* bit 7 of Port 3 */

/* Delay function */
void delay() {

        int i, j;

        for(i=0; i<1000; i++)
                for(j=0; j<100; j++)
                        i = i + 0;
}

/* Function to output the decimal value of the count on the LCD */
void PrintInt(unsigned char i) {
	char ch[4];

	/* Write code to convert the count to a string value and use the
           PrintString function provided in io.c */

        PrintString(ch);
}

void main(void) {

        unsigned char count = 0;

        InitIO();  /* Initialize the LCD */
        while (1) {
                if (Start_LCD == 1) {
                        ClearScreen();
                        PrintString("Ready...");
                        delay();
                }
                else if (reset == 1) {

                        /* Output 0 on the LCD */

                }
                else if (load == 1) {

                       /* Output the current value of Datain on the LCD */

                }
                else {
                        /* Check the Up/Down pin for 1 or 0 count up or down
                           accordingly. Display each value on the LCD */
               }
      }
}
