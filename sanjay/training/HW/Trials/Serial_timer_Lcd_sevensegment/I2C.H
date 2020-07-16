#ifndef __I2C_H_
#define __I2C_H_
	extern void I2C_Stop(void);
	extern void I2C_Start(void);
	extern void device_ACK(void);
	extern void Master_NO_ACK(void);
	extern void init_I2C(void);
	extern unsigned char Read_Data(void);
	extern void send_Data(unsigned char);

	#define send_Word_Address(x) send_Data(x)
	#define send_Device_Address(x) send_Data(x)
#endif
