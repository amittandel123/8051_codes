#include <reg51.h>
#include "declare.h"

enum MODE
{
	Randomly_Read	= 0,
	Cur_Add_Read 	= 1,
	Seqn_Read 		= 2
};

struct I2C_Param
{
	float T_BUF;
	float T_HD_STA;
	float T_LOW;
	float T_HIGH;
	float T_SU_STA;
	float T_HD_DAT;
	float TR;
	float TF;
	float T_SU_STO;
	enum MODE I2C_Mode;
	unsigned int bytes2read;
	unsigned char byte1_word_add,byte2_word_add,Dev_Address;
};
struct I2C_Param I2C_Info;


void wait(int time)
{
	time *=1000;
	while(time--)
	{
	}
}


void initialise(void)
{
	P0 =0xFF;
	P1 = 0xFF;

	SCON	= 0x50;
	TMOD	= 0x20;
	TI	= 1;
	TR1	= 1;
	TH1	= 0xFD;
	EA	= 0;
}

void current_address_read(unsigned char Device_Address)
{
	I2C_Info.I2C_Mode = Cur_Add_Read;
	I2C_Info.bytes2read = 1;
	I2C_Info.Dev_Address = Device_Address;
	Read_I2C();
}

void Random_Read(unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	I2C_Info.I2C_Mode = Randomly_Read;
	I2C_Info.byte1_word_add = byte1_Word_Address;
	I2C_Info.byte2_word_add = byte2_Word_Address;
	I2C_Info.bytes2read = 1;
	I2C_Info.Dev_Address = Device_Address;
	Read_I2C();
}

void Seq_Read(unsigned char Address_Device,unsigned char Word_Address_byte1, unsigned char Word_Address_byte2, unsigned int byte2read)
{
	unsigned char x;
	I2C_Info.I2C_Mode = Seqn_Read;
	I2C_Info.byte1_word_add = Word_Address_byte1;
	I2C_Info.byte2_word_add = Word_Address_byte2;
	I2C_Info.bytes2read = byte2read;
	I2C_Info.Dev_Address = Address_Device;
	x = Word_Address_byte2;
	Read_I2C();
}

void Write_Add_I2C(void)
{
	I2C_Start();	
	send_Device_Address(I2C_Info.Dev_Address); // write mode
	device_ACK();

	send_Word_Address(I2C_Info.byte1_word_add); // first byte of word address
	device_ACK();

	send_Word_Address(I2C_Info.byte2_word_add); // second byte of word address
	device_ACK();
}

void Read_I2C(void)
{
	unsigned int i=8,x;
	unsigned char readbyte = 0x00;
	if(I2C_Info.I2C_Mode != Cur_Add_Read)
	{
		Write_Add_I2C();	
	}
	for(x=0;x<I2C_Info.bytes2read;x++)
	{
		I2C_Start();	
		send_Device_Address((unsigned char)(I2C_Info.Dev_Address + 1)); // read mode
		device_ACK();
		readbyte= Read_Data();
//		if(x < (I2C_Info.bytes2read -1))//for linker
			Master_ACK();
		printf("%c",readbyte);
//		printf("%c at address : %X\n",readbyte,I2C_Info.byte1_word_add);
	}
	I2C_Stop();	
}

void byte_write(unsigned char mData,unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	unsigned int num_bytes;

	num_bytes = 1;
	I2C_Info.byte1_word_add = byte1_Word_Address;
	I2C_Info.byte2_word_add = byte2_Word_Address;
	I2C_Info.Dev_Address = Device_Address;

	Write_Add_I2C();

	send_Data(mData);			
	device_ACK();
	wait(15);
	I2C_Stop();	
}

void page_write(unsigned char byte[],unsigned char Device_Address,unsigned char byte1_Word_Address,unsigned char byte2_Word_Address)
{
	unsigned int num_bytes,i = 0;
	num_bytes = strlen(byte);
	I2C_Info.byte1_word_add = byte1_Word_Address;
	I2C_Info.byte2_word_add = byte2_Word_Address;
	I2C_Info.Dev_Address = Device_Address;

	Write_Add_I2C();
	for(i=0;i<num_bytes;i++)
	{
		send_Data(byte[i]);			
		device_ACK();
		wait(10);

		}
	I2C_Stop();	
}

void main(void)
{
	unsigned char y;
	unsigned char add=0x70;
	unsigned char xx = 0;
	unsigned char pagedata[]= {"SACHIN BAMANE 8051 par Ab main slow kaam karunga!!!!He heee........... "};

	initialise();
	I2C_initialise();	

	y=0x41;
	while(1)
	{
		
		if (xx>500)
		{	byte_write(y,0xA0,0x00,add); // (data,Dev_add,word1,word2)
			wait(20);
		}
//		printf("y=%d\n",(unsigned int)y); //self check : tracing
		if (xx>500)
			page_write(pagedata,0xA0,0x00,y); // (data,Dev_add,word1,word2)
		if (xx>500)
			set_Current_Address(0xA0, 0x00, 0x00);

//		if (xx>500)
		{
			I2C_Info.I2C_Mode = Cur_Add_Read;
			I2C_Info.byte1_word_add = 0x00;
			I2C_Info.byte2_word_add = add;
			I2C_Info.bytes2read = 1;
			I2C_Info.Dev_Address = 0xA0;
			Write_Add_I2C()	;
			current_address_read(0xA0);
		}
		if (xx>500)
			Random_Read(0xA0,0x00, add);
		if (xx>500)
			Seq_Read(0xA0,0x00,y,8);
		y++;
		add++;
		if(y>(0x41 + 26))
			while(1);
	}
}
