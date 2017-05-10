#include <timer_counter.h>

void timer_enable(TIM_TypeDef* timer)
{
	if(timer == TIM2)
	{
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	}
	else if(timer == TIM3)
	{
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
	}
}

/*
 * PSC: The times that core runs to have a counter
 * ARR: Auto-reload-register, reload after how many count
 */
void timer_init(TIM_TypeDef* timer, int psc, int arr)
{
	timer->PSC = (uint32_t)(psc-1);
	timer->ARR = (uint32_t)(arr-1);
	timer->EGR |= TIM_EGR_UG;
}

void timer_start(TIM_TypeDef* timer)
{
	timer->CR1 |= TIM_CR1_CEN;
}

void timer_stop(TIM_TypeDef* timer)
{
	timer->CR1 &= ~TIM_CR1_CEN;
}
