#include <reg51.h>
#include "I2C_EEPROM.H"
#include "TRANSMIT.H"
sbit WP = P2^0;

void Board_init(void)
{
	I2C_initialise();	
	initilise_rs232();
}

void main(void)
{
	unsigned char i, addr=0, Dev_Add = 0xA0, ReadByte;

	Board_init();

	while(1)
	{
		if(serial_flag)
		{
			serial_flag = 0;
			if(data_temp=='q' || data_temp == 'Q')
			{
				data_temp = 0;
				printstr("You have written : ");
				for(i=0; i<21; i++)
				{
					ReadByte = I2C_Read(Dev_Add, 0, i);
					print(ReadByte);
				}
			}
			else
			{
				transmit_data();
				I2C_Write(data_temp, 0xA0, 0x00, addr); // (data,Dev_add,word1,word2)
				wait(2);
				addr++;
				if(addr > 21) 
					addr = 0;
			}
		}
	}
}
