// Hex file for LCD

#include<reg51.h>

void Initialize_LCD();
void Display_Data_LCD(unsigned char*);
void Display_Char_LCD(unsigned char);
void Send_Command_LCD(unsigned char);
void Clear_LCD();
void Delay(unsigned int);
