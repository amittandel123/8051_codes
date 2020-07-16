
#include<reg51.h>

void tickDelay(unsigned int);	// Delay of 'tick X 1uS'
unsigned int Reset_iButton(void);
void Write_Bit(unsigned int);
unsigned int Read_Bit(void);
void Write_Byte(unsigned int);
unsigned int Read_Byte(void);
unsigned int Touch_Byte(unsigned int);
void Write_Block(unsigned char*, unsigned int);
