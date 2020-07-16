// Hex file for I2C

#include<reg51.h>

void send_Data(unsigned char str_data);
unsigned char Read_Data(void);
void Master_NO_ACK(void);
void device_ACK(void);
void I2C_Stop(void);
void I2C_Start(void);
void I2C_initialise(void);
