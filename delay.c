#include "stm32f0xx.h"

void delay (uint32_t ms)
{
	uint32_t nCount = (HSE_VALUE / 10000) * ms;
	
	for (; nCount != 0; --nCount)
		;
}
