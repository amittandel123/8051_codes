#include <reg51.h>
#include "SEVENSEG.H"
#include "TIMER0.H"
#include "LCD.H"
#include "TRANSMIT.H"
#include "EEPROM.H"

unsigned char LCD_DATA_1[][16] = {"**** Embedd ****", "                "};
unsigned char LCD_DATA_2[][16] = {"**** Sachin ****", "                "};
unsigned char Line1_Flag = 0, shift_counter = 0;
unsigned char Serial_Cnt=0, SevenSeg_Cnt = 0;
const unsigned char EEPROM_ADDR = 0xA0;

void board_init(void)
{
	initilise_rs232();
	init_timer0();
	init_sevenSeg();
	init_LCD();
	init_I2C();
}

void main(void)
{
	int i;
	board_init();
	writeCommand_LCD();

	for(i=0;i<16;i++)
	{
		LCD_DATA_2[0][i] = I2C_Read(EEPROM_ADDR, 0,i);
	}

	for(i=0;i<4;i++)
	{
		val[i] = I2C_Read(EEPROM_ADDR, 0,i+16);
	}
	
	SendCommand(0x80);// select line 2 on LCD
	delay(5);
	writeData_LCD(LCD_DATA_1[0],16);
	delay(5);
	SendCommand(0xC0);// select line 1 on LCD
	delay(5);

	writeData_LCD(LCD_DATA_2[0],16);
	delay(5);
		
	while(1)
	{
		display_string();
		if(serial_flag)
		{
			serial_flag = 0;
			LCD_DATA_2[0][Serial_Cnt] = data_temp;
			I2C_Write(data_temp, EEPROM_ADDR,0, Serial_Cnt);
			Serial_Cnt = ++Serial_Cnt % 16;
			
			val[SevenSeg_Cnt] = data_temp;
			SevenSeg_Cnt = ++SevenSeg_Cnt % 4;
			I2C_Write(data_temp, EEPROM_ADDR,0, SevenSeg_Cnt+16);

			transmit_data();
		}

		if(timer0_flag == 1)
		{
			timer0_flag = 0;
			tick_count = 0;
		 	if (Line1_Flag)
				Line1_Flag = 0;
			else
				Line1_Flag = 1;

			SendCommand(0x80);// select line 1 on LCD
			delay(5);
		
			writeData_LCD(LCD_DATA_1[Line1_Flag],16);
			delay(5);

			SendCommand(0xC0);// select line 1 on LCD
			delay(5);
		
			writeData_LCD(&LCD_DATA_2[0][shift_counter],16-shift_counter);
			delay(5);

			writeData_LCD(&LCD_DATA_2[1][0],shift_counter);
			delay(5);						
			shift_counter = shift_counter++ % 16;
		}
	}
}

