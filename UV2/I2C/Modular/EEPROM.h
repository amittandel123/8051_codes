#ifndef __EEPROM_H_
#define __EEPROM_H_
	#include "I2C.H"

	extern void DelayMs(unsigned int);

	#define send_Word_Address(x) send_Data(x)
	#define send_Device_Address(x) send_Data(x)
	
	void wait(unsigned int);
	extern unsigned char Read_I2C(void);
	extern void Write_Add_I2C(unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
	extern unsigned char I2C_Read(unsigned char Add_MSB,unsigned char Add_LSB);
	extern void I2C_Write(unsigned char mData,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
#endif


