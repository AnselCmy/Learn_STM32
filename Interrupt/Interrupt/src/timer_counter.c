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

void SystemClock_Config(int speed)
{
	// Set system clock -> MSI
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;
	RCC->CFGR |= RCC_CFGR_SW_MSI; // MIS clock
	// Confirm that system clock -> MSI
	while(!(((RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos) == 0));

	// The parameters must be changed under the condition of PLL off
	RCC->CR &= ~RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) != 0);

	// Set PLL to MSI
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC_Msk;
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_MSI;

	int set_R = 0, set_N = 0, set_M = 0;
	// 4*N / (2*(R+1) * (M+1))
	// 8<=N<=86, 2*(R+1)=2,4,6,8, 1<=M+1<=8
	if(speed == 40)
	{
		set_R = 1;
		set_N = 40;
		set_M = 0;
	}
	else if(speed == 16)
	{
		set_R = 0;
		set_N = 8;
		set_M = 0;
	}
	else if(speed == 10)
	{
		set_R = 0;
		set_N = 10;
		set_M = 1;
	}
	else if(speed == 6)
	{
		set_R = 0;
		set_N = 9;
		set_M = 2;
	}
	else if(speed == 1)
	{
		set_R = 3;
		set_N = 8;
		set_M = 3;
	}
	else if(speed == 6)
	{
		set_R = 3;
		set_N = 8;
		set_M = 0;
	}

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLR_Msk;
	RCC->PLLCFGR |= (set_R << RCC_PLLCFGR_PLLR_Pos);

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR |= (set_N << RCC_PLLCFGR_PLLN_Pos);

	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
	RCC->PLLCFGR |= (set_M << RCC_PLLCFGR_PLLM_Pos);

	// Enable PLLR
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;

	// Set system clock -> PLL
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	// Confirm use PLL as system clock
	while(!(((RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos) == 3));
}

void SystemClock_Config_Interrupt(int speed, int load)
{
	SystemClock_Config(speed);
	SysTick->LOAD = load;
	SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos);
	SysTick->CTRL |= (1 << SysTick_CTRL_TICKINT_Pos);
	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);
}

void GPIO_init_AF()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Set alternate function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOA->MODER |= (2 << GPIO_MODER_MODE0_Pos);

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos);
}

void PWM_channel_init()
{
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
	TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);

	TIM2->CCMR1 &= ~TIM_CCMR1_OC1PE_Msk;
	TIM2->CCMR1 |= (1 << TIM_CCMR1_OC1PE_Pos);

	TIM2->CCER &= ~TIM_CCER_CC1P_Msk;
	TIM2->CCER |= (0 << TIM_CCER_CC1P_Pos);

	TIM2->CCER &= ~TIM_CCER_CC1E_Msk;
	TIM2->CCER |= (1 << TIM_CCER_CC1E_Pos);

	TIM2->CCR1 = 10;
	TIM2->PSC = 0;
}
