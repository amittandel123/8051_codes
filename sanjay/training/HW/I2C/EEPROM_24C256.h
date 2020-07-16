extern void wait(int time);
extern void Write_Add_I2C(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
extern unsigned char Read_I2C(unsigned char Dev_Address);
extern void I2C_Write(unsigned char mData,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
extern unsigned char I2C_Read(unsigned char Dev_Add,unsigned char Add_MSB,unsigned char Add_LSB);
