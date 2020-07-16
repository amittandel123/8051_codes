#ifndef __I2C_H_
#define __I2C_H_
	void I2C_Stop(void);
	void I2C_Start(void);
	void device_ACK(void);
	void Master_NO_ACK(void);
	void I2C_initialise(void);
	unsigned char Read_Data(void);
	void send_Data(unsigned char);
	#define send_Word_Address(x) send_Data(x)
	#define send_Device_Address(x) send_Data(x)
#endif
