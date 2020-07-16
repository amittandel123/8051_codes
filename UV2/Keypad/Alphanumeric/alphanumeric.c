// Program to make alphabetical keypad similar to the one in mobiles

#include<reg51.h>
#define port P1
#define dataport P2  //Data port for LCD
#define key P0  	// Port for Keypad
#define sec 100
sbit rs = port^1;
sbit rw = port^2;
sbit en = port^3;

sbit col1=key^6;
sbit col2=key^5;
sbit col3=key^4;
sbit row1=key^0;
sbit row2=key^1;
sbit row3=key^2;
sbit row4=key^3;

unsigned char position=128,i=0;
int check_again[10]={0,0,0,0,0,0,0,0,0,0};

void delay(unsigned int msec) // Time delay function
{
	int i,j;
	for(i=0;i<msec;i++)
	  for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char item)
{
	dataport=item;
	rs=0;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
}

void lcd_data(unsigned char item)
{
	lcd_cmd(0x0f);
	delay(10);
	dataport=item;
	rs=1;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
}

void num_check(int a, int b)  //Check function
{
	int j;
	for(j=0;j<10;j++)
	{
	   if(j!=a)
	   {
	   		check_again[j]=0;
	   }
	}
	if(check_again[a]==3)  	// check if 3 times same key press occur
		check_again[a]=0;	// reset counter to 0
	check_again[11]=1;
	check_again[12]=1;
}

void display(int a, int b) // Display function
{
	switch(b)
	{
		case 1:
	    {
		    switch(a)
		    {
			     case 1:
				 {
					 if(check_again[a]>1)
					 	i--;
					 lcd_cmd(position+i);
					 lcd_data('a');
					 delay(5);
					 num_check(1 ,check_again[1]);
					 break;
			     }
			     case 2:
				 {
				     if(check_again[a]>1)
					     i--;
				     lcd_cmd(position+i);
				     lcd_data('d');
				     delay(5);
				     num_check(2 ,check_again[2]);
				     break;
			     }
			     case 3: 
				 {
				       if(check_again[a]>1)
					       i--;
				       lcd_cmd(position+i);
				       lcd_data('g');
				       delay(5);
				       num_check(3 ,check_again[3]);
				       break;
			     }
			     case 4:
				 {
				       if(check_again[a]>1)
				 	      i--;
				       lcd_cmd(position+i);
				       lcd_data('j');
				       delay(5);
				       num_check(4 ,check_again[4]);
				       break;
			     }
			     case 5:
				 {
				       if(check_again[a]>1)
				 	      i--;
				       lcd_cmd(position+i);
				       lcd_data('m');
				       delay(5);
				       num_check(5 ,check_again[5]);
				       break;
			     }
			     case 6:
				 {
				       if(check_again[a]>1)
				 	      i--;
				       lcd_cmd(position+i);
					       lcd_data('p');
				       delay(5);
				       num_check(6 ,check_again[6]);
				       break;
			     }
			     case 7:
				 {
				       if(check_again[a]>1)
				       i--;
				       lcd_cmd(position+i);
					       lcd_data('s');
				       delay(5);
				       num_check(7 ,check_again[7]);
				       break;
			     }
			     case 8:
				 {
				       if(check_again[a]>1)
				       i--;
				       lcd_cmd(position+i);
					       lcd_data('v');
				       delay(5);
				       num_check(8 ,check_again[8]);
				       break;
			     }
			     case 9:
				 {
				       if(check_again[a]>1)
				       i--;
				       lcd_cmd(position+i); 
				 	      lcd_data('y');
				       delay(5);
				       num_check(9 ,check_again[9]);
				       break;
			     }
			     case 0:
				 { 
				       lcd_cmd(position+i);
				       lcd_data(' ');
				       delay(5);
				       num_check(10 ,check_again[10]);
				       break;
			     }
			     case 11:
				 {
				       if((position+i)>129)
				       {
					       i=i-2;
					       delay(5);
					       lcd_cmd(position+i+1);
					       num_check(11 ,check_again[11]);;
					       delay(5);
			       	   }
			       	   break;
			     }
			     case 12:
				 {
				       delay(5);
				       lcd_cmd(position+i);
				       num_check(12 ,check_again[12]);
				       delay(5);
				       break;
		       	 }
		    }
	   		break;
	   }
	
	  case 2:
	  {
	    	 switch(a)
		     {
			     case 1:
				 {
			       if(check_again[a]>1)
			    	   i--;
			       lcd_cmd(position+i);
			       lcd_data('b');
			       delay(5);
			       num_check(1 ,check_again[1]);
			       break;
			     }
			     case 2:
				 {
			       if(check_again[a]>1)
			   	    i--;
			       lcd_cmd(position+i);
			       lcd_data('e');
			       delay(5);
			       num_check(2 ,check_again[2]);
			       break;
			     }
			     case 3: {
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('h');
			       delay(5);
			       num_check(3 ,check_again[3]);
			       break;
			       }
			     case 4:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('k');
			       delay(5);
			       num_check(4 ,check_again[4]);
			       break;
			       }
			     case 5:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('n');
			       delay(5);
			       num_check(5 ,check_again[5]);
			       break;
			       }
			     case 6:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('q');
			       delay(5);
			       num_check(6 ,check_again[6]);
			       break;
			       }
			     case 7:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('t');
			       delay(5);
			       num_check(7 ,check_again[7]);
			       break;
			       }
			     case 8:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i);
			       lcd_data('w');
			       delay(5);
			       num_check(8 ,check_again[8]);
			       break;
			       }
			     case 9:{
			       if(check_again[a]>1)
			       i--;
			       lcd_cmd(position+i); 
			       lcd_data('z');
			       delay(5);
			       num_check(9 ,check_again[9]);
			       break;
			       }
			     case 0:{
			       lcd_cmd(position+i);
			       lcd_data('-');
			       num_check(10 ,check_again[10]);
			       break;
			       }
			     case 11:{
			       i--;
			       lcd_cmd(0x10);
			       delay(5);
			       break;
			       }
		     }
	    break;
	   }

	  case 3:
	  {
	   		switch(a)
	     	{
			     case 1:{
			       if(check_again[a]>1)
				       i--;
			       lcd_cmd(position+i);
			       lcd_data('c');
			       delay(5);
			       num_check(1 ,check_again[1]);
			       break;
			       }
			     case 2:{
			       if(check_again[a]>1)
			   		    i--;
			       lcd_cmd(position+i);
			       lcd_data('f');
			       delay(5);
			       num_check(2 ,check_again[2]);
			       break;
			       }
			     case 3: {
			       if(check_again[a]>1)
			   		    i--;
			       lcd_cmd(position+i);
			       lcd_data('i');
			       delay(5);
			       num_check(3 ,check_again[3]);
			       break;
			       }
			     case 4:{
			       if(check_again[a]>1)
			   		    i--;
			       lcd_cmd(position+i);
			       lcd_data('l');
			       delay(5);
			       num_check(4 ,check_again[4]);
			       break;
			       }
			     case 5:{
			       if(check_again[a]>1)
			   		    i--;
			       lcd_cmd(position+i);
			       lcd_data('o');
			       delay(5);
			       num_check(5 ,check_again[5]);
			       break;
			       }
			     case 6:{
			       if(check_again[a]>1)
			  		     i--;
			       lcd_cmd(position+i);
			       lcd_data('r');
			       delay(5);
			       num_check(6 ,check_again[6]);
			       break;
			       }
			     case 7:{
			       if(check_again[a]>1)
			    		   i--;
			       lcd_cmd(position+i);
			       lcd_data('u');
			       delay(5);
			       num_check(7 ,check_again[7]);
			       break;
			       }
			     case 8:{
			       if(check_again[a]>1)
			  		     i--;
			       lcd_cmd(position+i);
			       lcd_data('x');
			       delay(5);
			       num_check(8 ,check_again[8]);
			       break;
			       }
			     case 9:{
			       if(check_again[a]>1)
			 		      i--;
			       lcd_cmd(position+i); 
			       lcd_data('z');
			       delay(5);
			       num_check(9 ,check_again[9]);
			       break;
			       }
			     case 0:{  lcd_cmd(position+i);
			       lcd_data('-');
			       num_check(10 ,check_again[10]);
			       break;
			      
			       }
			     case 11:{
			       i=i-2;
			       lcd_cmd(position+i);
			       delay(5);
			       break;
			       }
	     }
	   break;
	   }
	}
}

void check_col1()   //Check column one
{
	row1=row2=row3=row4=1;
	row1=0;
	if(col1==0)
	{
	  delay(10);
	  check_again[1]++;
	  display(1,check_again[1]);
	}
	row1=1;
	row2=0;
	if(col1==0)
	{
	  delay(10);
	  check_again[4]++;
	  display(4,check_again[4]);
	}
	row2=1;
	row3=0;
	if(col1==0)
	{
	  delay(10);
	  check_again[7]++;
	  display(7,check_again[7]);
	}
	row3=1;
	row4=0;
	if(col1==0)
	{
	  delay(10);
	  display(11,1);  //FOR *
	}
	row4=1;
}

void check_col2() //Check column two
{
	row1=row2=row3=row4=1;
	row1=0;
	if(col2==0)
	{
	  delay(10);
	  check_again[2]++;
	  display(2,check_again[2]);
	}
	row1=1;
	row2=0;
	if(col2==0)
	{
	  check_again[5]++;
	  display(5,check_again[5]);
	}
	row2=1;
	row3=0;
	if(col2==0)
	{
	  delay(10);
	  check_again[8]++;
	  display(8,check_again[8]);
	}
	row3=1;
	row4=0;
	if(col2==0)
	{
	  delay(10);
	  check_again[0]++;
	  display(0,1);
	}
	row4=1;
}

void check_col3() //check column three
{
	row1=row2=row3=row4=1;
	row1=0;
	if(col3==0)
	{
	  delay(10);
	  check_again[3]++;
	  display(3,check_again[3]);
	}
	row1=1;
	row2=0;
	if(col3==0)
	{
	  delay(10);
	  check_again[6]++;
	  display(6,check_again[6]);
	}
	row2=1;
	row3=0;
	if(col3==0)
	{
	  delay(10);
	  check_again[9]++;
	  display(9,check_again[9]);
	}
	row3=1;
	row4=0;
	if(col3==0)
	{
	  delay(10);
	  display(12,1);  //FOR #
	}
	row4=1;
}

void keypad1()
{
	if((position+i)>144 && i>16)
	{
	  position=192;
	  i=0;
	}
	if((position+i)>203 && i>12 )
	{
	  lcd_cmd(0X01);
	  i=0;
	  position=128;
	}
	i++;
	lcd_cmd(position+i);
	if(col1==0)
		check_col1();
	else if(col2==0)
	    check_col2();
	else if(col3==0)
	    check_col3();
	delay(10);
}

void main()
{
	col1=col2=col3=1;
	lcd_cmd(0x38);
	lcd_cmd(0x0C);
	lcd_cmd(0x01);
	delay(sec);
	while(1)
	{
		  row1=row2=row3=row4=0;
		  while(col1==1 && col2==1 && col3==1);
		  {
		   		keypad1();
		  }
	}
}
