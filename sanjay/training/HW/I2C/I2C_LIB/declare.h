#ifndef __DECLARE_H_
#define __DECLARE_H_
	#define send_Word_Address(x) send_Data(x)
	#define send_Device_Address(x) send_Data(x)
	extern void I2C_Stop(void);
	extern void I2C_Start(void);
	extern void device_ACK(void);
	extern void Master_NO_ACK(void);
	extern void I2C_initialise(void);
	extern unsigned char Read_Data(void);
	extern void send_Data(unsigned char);
	extern unsigned char RTC_Read(unsigned char);
	extern void RTC_Write(unsigned char, unsigned char);
//	extern void send_Word_Address(unsigned char);
//	extern void send_Device_Address(unsigned char);
	
	void wait(int);
	unsigned char Read_I2C(unsigned char);
	void Write_Add_I2C(unsigned char, unsigned char,unsigned char);
	unsigned char I2C_Read(unsigned char, unsigned char, unsigned char);
	void I2C_Write(unsigned char, unsigned char, unsigned char, unsigned char);
#endif
