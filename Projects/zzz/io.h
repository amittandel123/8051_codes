#ifndef __io_h__
#define __io_h__

#define HIDE_CURSOR	0,25

void InitIO(void);
void ClearScreen(void);
void GotoXY(unsigned char r, unsigned char c);
void PutChar(char c);
void PrintString(const char* s);

#endif
