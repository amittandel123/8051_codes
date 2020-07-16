#include <reg51.h>
#include "EEPROM.H"

void wait(int time)
{
	time *=1000;
	while(time--)
	{
	}
}

void Write_Add_I2C(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	I2C_Start();	
	send_Device_Address(Device_Address); // write mode
	device_ACK();

	send_Word_Address(byte1_Word_Address); // first byte of word address
	device_ACK();

	send_Word_Address(byte2_Word_Address); // second byte of word address
	device_ACK();
}

unsigned char Read_I2C(unsigned char Dev_Address)
{
	unsigned char Readbyte;
	I2C_Start();	
		send_Device_Address(Dev_Address + 1); // read mode
		device_ACK();
		Readbyte = Read_Data();
		Master_NO_ACK();
	I2C_Stop();	
	return(Readbyte);
}

void I2C_Write(unsigned char mData,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	Write_Add_I2C(Device_Address,byte1_Word_Address, byte2_Word_Address);

	send_Data(mData);			
	device_ACK();
	I2C_Stop();		  
	wait(1);
}

unsigned char I2C_Read(unsigned char Dev_Add,unsigned char Add_MSB,unsigned char Add_LSB)
{
	Write_Add_I2C(Dev_Add,Add_MSB,Add_LSB);
	return(Read_I2C(Dev_Add));
}
