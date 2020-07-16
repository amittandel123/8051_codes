#include <reg51.h>

#define DISP_PORT P0
#define CNTRL_PORT P1
unsigned int i;
unsigned char mData;

void main(void)
{
	mData = 0xFE;
	CNTRL_PORT=0xF0;

	while(1)
	{
		DISP_PORT = ~mData;
		for(i=0;i<30000;i++);

		if (mData == 0x00)
			mData = 0xFF;

		mData = mData<<1;
	}
}


