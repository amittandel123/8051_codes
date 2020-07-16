#ifndef __LCD_H__
#define __LCD_H__

	extern void move_cursor(int Direction, unsigned int ChangeCount);
	extern void multi_char_key(void);
	extern void display_data(unsigned char port_data);
	extern void writeData_LCD(unsigned char mdata[], unsigned int counter);
	extern void ClearDisplay(void);
	extern void SendCommand(unsigned char comd);
	extern void Send(unsigned char comd);
	extern void GenChar_at_CGRAM(unsigned char mdata[]);
	extern void writeCommand_LCD(void);
	extern void initialise_LCD(void);
#endif
