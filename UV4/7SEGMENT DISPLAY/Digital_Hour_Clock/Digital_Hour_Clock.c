// Program to demonstrate the principle of digital clock. this digital clock displays two minute digits and two second digits

#include<reg51.h>
 sbit dig_ctrl_4=P1^0;   //Declaring control pins of the seven segments
 sbit dig_ctrl_3=P1^1;
 sbit dig_ctrl_2=P1^2;
 sbit dig_ctrl_1=P1^3;
 unsigned char dig_disp=0;
 int min2;
 int min1;
 int sec2;
 int sec1;
 char digi_val[10]={0x40,0xF9,0x24,0x30,0x19,0x12,0x02,0xF8,0x00,0x10};

void delay()	//Function to provide a time delay of approx. 1 second. using Timer 1.
{
    int i;
    for(i=0;i<20;i++)
    {
         TL1=0xFD;
         TH1=0x4B;
         TR1=1;
         while(TF1==0);
         TR1=0;
         TF1=0;
    }
}

void display() interrupt 1        //Function to display the number using seven segmnet multiplexing. For more details refer seven segment multiplexing.
{
    TL0=0x36;        //Reloading Timer0
    TH0=0xf6;
    P2=0xFF;
    dig_ctrl_1 = dig_ctrl_3 = dig_ctrl_2 = dig_ctrl_4 = 0;
    dig_disp++;
    dig_disp=dig_disp%4;
   
    switch(dig_disp)
    {
        case 0:
        P2=digi_val[sec1];
        dig_ctrl_1 = 1;
        break;
   
        case 1:
        P2=    digi_val[sec2];
        dig_ctrl_2 = 1;
        break;
   
        case 2:
        P2=    digi_val[min1];
        dig_ctrl_3 = 1;
        break;
   
        case 3:
        P2=    digi_val[min2];
        dig_ctrl_4 = 1;
        break;
    }
}

void main()
{
    TMOD=0x11;        //Intialize Timer 0
    TL0=0x36;
    TH0=0xF6;
    IE=0x82;        // Enable Timer 0 interrupt
    TR0=1;        //Start Timer 0
    while(1)        //Start clock
    {
          min2=min1=sec2=sec1=0;
          for(min2=0;min2<6;min2++)
          {
               for(min1=0;min1<10;min1++)
               {
               		for(sec2=0;sec2<6;sec2++)
                	{
                    	 for(sec1=0;sec1<10;sec1++)
                     	 {
                         	delay();
                   		 }
                	}
               }
          }
     }
}
