#include <reg51.h>
#include "COMMON.H"

void delay(unsigned int val)
{
	unsigned int u,v;
	for(u=0;u<=val;u++)
		for(v=0;v<=100;v++);
}

