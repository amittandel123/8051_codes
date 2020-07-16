#include<reg51.h>
sbit sensor=P1^0;
sbit count=P0^1;
sbit LED=P0^2;
unsigned int i,x,j;
void delay (unsigned int x);
void main()
{
        unsigned char mydata[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
		P2=0xAA;												         //homing of all cylinders
		P0=0x00;												         // Initially no input to LED and counter
		P1=0X00;												         //Clear the sensor input
																        
        while(1)                                                       //Do it continiously  
		 {

            for(i=0;i<=7;i++)	                                         //cylinder operation
                {
                          P2=mydata[i];                                  //port pin shifting
                          delay(40);										 //Delay
	    		}
						 
                          P2=0x00;					                      //deactivating all cylinders
               				  
					      P1=0X01;										 //Fake sense,Testing purpose
				   
				   
		     if(sensor==1)		                                         //read the proximity switch
                {
                          
						  count=1;
						  delay(0);		                                 // high pulse to the counter
                          count=0;
						  delay(0);
						
						  
						  
						   for(i=0;i<=7;i++)	                          //cylinder operation
                               {
                                       P2=mydata[i];                       //port pin shifting
                                       delay(40);
						       }
						 
                      
					                    P2=0x00;					     //deactivating all cylinders
                 
				 
				               {
				                       
									   P1=0X00;							  //Fake sense,Testing purpose
				                       
									   
									   {
									   if(sensor==0);					   //Read the proximity sensor
				                     
									    else							   //error case
				                          {
					                          LED=1;
					                          while(1);
					                      }
					 	                }
					             }
				}
				
                   
				   
		else            			                                        //error case
	     {      
		 
		       LED=1;
			   while(1);
 		  
		 }
          
		  
	  }
}
	
	
	
	
	
void delay(unsigned int x)	                                                        //delay subroutine
{
     TMOD=0x01;														   //Timer 0,Mode 1
 
      TH0=0xFC;
        TL0=0x18;
  for(j=0;j<=x;j++)
	{
	 TR0=1;																//Start the timer 
     while(TF0==0);														// wait till overflow
     TR0=0;															   // Stop the timer
     TF0=0;												 
	 }
}
