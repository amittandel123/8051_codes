#ifndef __I2C_H_
#define __I2C_H_
	extern unsigned char I2C_Read(unsigned char Dev_Add,unsigned char Add_MSB,unsigned char Add_LSB);
	extern void I2C_Write(unsigned char mData,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
#endif
