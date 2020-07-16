#include <stdio.h>
#include <intrins.h>
#include <string.h>
sbit SDA = P1^7;
sbit SCL = P1^6;

extern bit HIGH,LOW;
//extern struct I2C_Param I2C_Info;

extern void device_ACK(void);
extern void Master_ACK(void);
extern void I2C_Stop(void);
extern void I2C_Start(void);
extern void I2C_initialise(void);
extern unsigned char Read_Data(void);
extern void send_Data(unsigned char);
extern void send_Word_Address(unsigned char);
extern void send_Device_Address(unsigned char);
extern void set_Current_Address(unsigned char, unsigned char, unsigned char);

void Read_I2C(void);
void initialise(void);
void Write_Add_I2C(void);
extern unsigned char current_address_read(unsigned char Device_Address);
extern unsigned char Random_Read(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
void byte_write(unsigned char byte,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
void page_write(unsigned char byte[],unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address);
void Seq_Read(unsigned char Address_Device,unsigned char Word_Address_byte1, unsigned char Word_Address_byte2, unsigned int byte2read);

