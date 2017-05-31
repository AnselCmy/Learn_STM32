#include "helper_functions.h"

void delay_without_interrupt(float msec)
{
	int loop_cnt = 500*msec;
	while(loop_cnt)
	{
		loop_cnt--;
	}
	return;
}

void FPU_init()
{
	SCB->CPACR |= (0xf << 20);
	__DSB();
	__ISB();
}
