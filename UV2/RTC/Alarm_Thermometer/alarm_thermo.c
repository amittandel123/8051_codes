// Program to make a digital clock with integrated Alarm and digital thermometer
#include<reg51.h>
#define port P1
#define adc_input P0
#define cont_port P3
#define dataport P2
#define m_sec 10

sbit rs = cont_port^0;
sbit rw = cont_port^1;
sbit en = cont_port^6;
sbit dig_hr1=port^0;
sbit dig_min1=port^1;
sbit start=port^2;
sbit am_pm=port^3;
sbit alarm_set=port^4;
sbit alarm=port^7;
sbit wr= P3^2;
sbit rd= P3^3;
sbit intr= P3^4;

int hr ,hr1=0,alarm_hr=0;
int min,min1=0,alarm_min=0;
int sec,sec1=0,dig_am_pm=0,alarm_am_pm=0;
int test_final=0,test_intermediate1[10],test_intermediate2[3]={0,0,0};

void delay(unsigned int msec) // Time dealy function
{
	int i,j ;
	for(i=0;i<msec;i++)
	  for(j=0;j<1275;j++);
}

void lcd_cmd(unsigned char item)  //Function to send command to LCD           
{
	dataport = item;
	rs= 0;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
}

void lcd_data(unsigned char item) // Function to send data to LCD
{
	dataport = item;
	rs= 1;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
}

void lcd_data_string(unsigned char *str) // Function to send string to LCD
{
	int i=0;
	while(str[i]!='\0')
	{
		  lcd_data(str[i]);
		  i++;
		  delay(1);
	}
	return;
}

lcd_data_int(int time_val)  // Function to send number to LCD
{
	int int_amt;
	int_amt=time_val/10;
	lcd_data(int_amt+48);
	int_amt=time_val%10; 
	lcd_data(int_amt+48);
	return;
}

void lcd(unsigned char str1[10])
{
	lcd_cmd(0x38); 
	lcd_cmd(0x0e); 
	delay(1);
	lcd_data_string(str1);
}

void shape()  // Function to create the shape of degree
{
	lcd_cmd(64);
	lcd_data(2);
	lcd_data(5);
	lcd_data(2);
	lcd_data(0);
	lcd_data(0);
	lcd_data(0);
	lcd_data(0);
	lcd_data(0);
}

void convert()  // Function to convert the data of ADC
{
	int s;
	s=test_final/100;
	test_final=test_final%100;
	lcd_cmd(0xc9);
	if(s!=0)
		lcd_data(s+48);
	else
		lcd_cmd(0x06);
	s=test_final/10;
	test_final=test_final%10;
	lcd_data(s+48);
	lcd_data(test_final+48);
	lcd_data(0);
	lcd_data('C');
	lcd_data(' ');
	delay(2);
}

void set_hr1()  // Function to set set hours digit of clock
{
	hr1++;
	if(hr1>11)
		hr1=0;
	lcd_cmd(0xc3);
	lcd_data_int(hr1);
	lcd_data(':');
}

void set_min1()  // Function to set set minutes digit of clock
{
	min1++;
	if(min1>59)
		min1=0;
	lcd_cmd(0xc6);
	lcd_data_int(min1);
}

void set_alarm_hr1() // Function to set set hours digit of alarm
{
	alarm_hr++;
	if(alarm_hr>11)
		alarm_hr=0;
	lcd_cmd(0xc3);
	lcd_data_int(alarm_hr);
	lcd_data(':');
}

void set_alarm_min1() // Function to set set minutes digit of clock
{
	alarm_min++;
	if(alarm_min>59)
		alarm_min=0;
	lcd_cmd(0xc6);
	lcd_data_int(alarm_min);
}

void alarm_check()  // Function to check alarm
{
	if(hr==alarm_hr)
	{
		if(min==alarm_min)
		{
			if(dig_am_pm==alarm_am_pm)
			{
				alarm=1;
				lcd_cmd(0x8b);
				lcd("ALARM");
			}
		}
	} 
}

void temp() // Function to calculate temperature
{
	int i;
	for(i=0;i<10;i++)
	{
		delay(1);
		rd=1;
		wr=0;
		delay(1);
		wr=1;
		while(intr==1);
		rd=0;
		lcd_cmd(0x88);
		test_intermediate1[i]=adc_input/10;
		delay(1);
		intr=1;
	}
	for(i=0;i<10;i++)
		test_intermediate2[0]=test_intermediate1[i]+test_intermediate2[0];
	for(i=0;i<10;i++)
	{
		delay(1);
		rd=1;
		wr=0;
		delay(1);
		wr=1;
		while(intr==1);
		rd=0;
		lcd_cmd(0x88);
		test_intermediate1[i]=adc_input/10;
		delay(1);
		intr=1;
	}
	for(i=0;i<10;i++)
		test_intermediate2[1]=test_intermediate1[i]+test_intermediate2[1];
	for(i=0;i<10;i++)
	{
		delay(1);
		rd=1;
		wr=0;
		delay(1);
		wr=1;
		while(intr==1);
		rd=0;
		lcd_cmd(0x88);
		test_intermediate1[i]=adc_input/10;
		delay(1);
		intr=1;
	}
	for(i=0;i<10;i++)
		test_intermediate2[2]=test_intermediate1[i]+test_intermediate2[2];
	test_intermediate2[0]=test_intermediate2[0]/3;
	test_intermediate2[1]=test_intermediate2[1]/3;
	test_intermediate2[2]=test_intermediate2[2]/3;
	test_final=test_intermediate2[0]+test_intermediate2[1]+test_intermediate2[2];
	shape();
	convert();
}

void main()
{
	int k;
	start=1;
	dig_hr1=1;
	dig_min1=1;
	alarm_set=1;
	alarm=0;
	lcd_cmd(0x83);
	lcd("SET ALARM");
	lcd_cmd(0xc3);
	lcd_data_int(hr1);
	lcd_data(':');
	lcd_data_int(min1);
	while(alarm_set==0)
	{
		delay(10);
		if(dig_hr1==0)
			set_alarm_hr1();
		if(dig_min1==0)
			set_alarm_min1();
	}
	
	if(am_pm==0)
	{
		lcd_cmd(0xc8);
		lcd_data_string("am");
		alarm_am_pm=0;
	}
	
	if(am_pm==1)
	{
		lcd_cmd(0xc8);
		lcd_data_string("pm");
		alarm_am_pm=1;
	}
	delay(200);
	lcd_cmd(0x01);
	lcd_cmd(0x83);
	lcd("SET TIMING");
	lcd_cmd(0xc3);
	lcd_data_int(hr1);
	lcd_data(':');
	lcd_data_int(min1);
	while(start==0)
	{
		delay(10);
		if(dig_hr1==0)
			set_hr1();
		if(dig_min1==0)
			set_min1(); 
	}
	
	if(am_pm==0)
	{
		lcd_cmd(0xc8);
		lcd_data_string("am");
		dig_am_pm=0;
	}
	
	if(am_pm==1)
	{
		lcd_cmd(0xc8);
		lcd_data_string("pm");
		dig_am_pm=1;
	}
	delay(200);
	lcd_cmd(0x01);
	while(1)
	{
		for(k=0;k<2;k++)
		{
			for(hr=hr1;hr<12;hr++)
			{
				for(min=min1;min<60;min++)
				{
					for(sec=0;sec<60;sec++)
					{
						lcd_cmd(0x81);
						delay(1);
						lcd_data_int(hr);
						lcd_data(':');
						lcd_data_int(min);
						lcd_data(':');
						lcd_data_int(sec);
						if(dig_am_pm==0)
						{
							lcd("am");
						}
						else
						{
							lcd("pm");
						}
						alarm_check();
						lcd_cmd(0xc3); 
						delay(2);
						lcd_data_string("TEMP:");
						temp();
						lcd_data_string("    ");
					}
				}
				min1=0;
			}
			if(dig_am_pm==0)
				dig_am_pm=1;
			else
				dig_am_pm=0;
			hr1=0;
		}
	}
}
