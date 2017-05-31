#include "stm32l476xx.h"
#include "gpio.h"
#include "led_button.h"
#include "7seg.h"
#include "keypad.h"
#include "timer_counter.h"
#include "helper_functions.h"

#define LED_gpio GPIOA
#define LED_pin 5

#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

#define COUNTER_timer TIM2

#define LAB1
//#define LAB2
//#define LAB3

#ifdef LAB1
void SysTick_Handler()
{
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
	{
		set_gpio(LED_gpio, LED_pin);
		delay_without_interrupt(100);
		reset_gpio(LED_gpio, LED_pin);
	}
}

int main()
{
	FPU_init();

	if(init_led(LED_gpio, LED_pin) != 0)
	{
		return -1;
	}
	if(init_button(BUTTON_gpio, BUTTON_pin) != 0)
	{
		return -1;
	}

	SystemClock_Config_Interrupt(10, 5000000);

	int button_press_cycle_per_second = 20;
	int debounce_cycles = 50;
	int debounce_threshold = debounce_cycles*0.7;
	int last_button_state = 0;

	while(1)
	{
		for(int a=0; a<button_press_cycle_per_second; a++)
		{
			int pos_cnt=0;
			for(int a=0; a<debounce_cycles; a++)
			{
				if(read_gpio(BUTTON_gpio, BUTTON_pin) == 0)
				{
					pos_cnt++;
				}
				delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
			}

			if(pos_cnt > debounce_threshold)
			{
				if(last_button_state == 0)
				{
					SysTick->CTRL ^= (1 << SysTick_CTRL_ENABLE_Pos);
				}
				last_button_state = 1;
			}
			else
			{
				last_button_state = 0;
			}
		}
	}

	while(1){}

	return 0;
}
#endif


#ifdef LAB2
int keypad[4][4] = {{1, 2, 3, 10},
					{4, 5, 6, 11},
					{7, 8, 9, 12},
					{15, 0, 14, 13}};

int COL_pin[] = {x0, x1, x2, x3};
int ROW_pin[] = {y0, y1, y2, y3};

int now_col_idx = 3;
int keyCnt = 0, keyValue = -1;



void SysTick_Handler()
{
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
	{
//		reset_gpio(PORT_COL, COL_pin[now_col_idx]);
		now_col_idx = (now_col_idx+1) % 4;
//		set_gpio(PORT_COL, COL_pin[now_col_idx]);
		GPIOA->ODR = (GPIOA->ODR & 0xFFFFE8FF) | 1 << COL_pin[now_col_idx];
//		set_gpio(LED_gpio, LED_pin);
//		delay_without_interrupt(100);
//		reset_gpio(LED_gpio, LED_pin);
	}
}

void EXTI_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5_Msk;
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI5_Pos); // PB5
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6_Msk;
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI6_Pos); // PB6
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI7_Msk;
	SYSCFG->EXTICR[1] |= (1 << SYSCFG_EXTICR2_EXTI7_Pos); // PB7
	SYSCFG->EXTICR[2] &= ~SYSCFG_EXTICR3_EXTI9_Msk;
	SYSCFG->EXTICR[2] |= (1 << SYSCFG_EXTICR3_EXTI9_Pos); // PB9

	EXTI->IMR1 |= EXTI_IMR1_IM5;
	EXTI->IMR1 |= EXTI_IMR1_IM6;
	EXTI->IMR1 |= EXTI_IMR1_IM7;
	EXTI->IMR1 |= EXTI_IMR1_IM9;

	EXTI->FTSR1 |= EXTI_FTSR1_FT5;
	EXTI->FTSR1 |= EXTI_FTSR1_FT6;
	EXTI->FTSR1 |= EXTI_FTSR1_FT7;
	EXTI->FTSR1 |= EXTI_FTSR1_FT9;

//	NVIC_EnableIRQ(EXTI0_IRQn);
//	NVIC_EnableIRQ(EXTI1_IRQn);
//	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTIKeypadHandler(int r)
{
	int nowKey = keypad[r][(now_col_idx+3)%4];
	if(nowKey == keyValue)
	{
		keyCnt++;
	}
	else
	{
		keyCnt = 0;
	}
	keyValue = nowKey;
	if(keyCnt >= 5)
	{
		keyCnt = 5;
		display_from_right(keyValue);
	}
}

void EXTI9_5_IRQHandler()
{
	if(EXTI->PR1 & EXTI_PR1_PIF5_Msk)
	{
		EXTIKeypadHandler(0);
		EXTI->PR1 = EXTI_PR1_PIF5_Msk;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF6_Msk)
	{
		EXTIKeypadHandler(1);
		EXTI->PR1 = EXTI_PR1_PIF6_Msk;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF7_Msk)
	{
		EXTIKeypadHandler(2);
		EXTI->PR1 = EXTI_PR1_PIF7_Msk;
	}
	if(EXTI->PR1 & EXTI_PR1_PIF9_Msk)
	{
		EXTIKeypadHandler(3);
		EXTI->PR1 = EXTI_PR1_PIF9_Msk;
	}
}

int main()
{
	FPU_init();

	init_led(LED_gpio, LED_pin);

	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Decode(0x00);
	set_ScanLimit(0x07);
	set_Shutdown(0x01);
	set_Intensity(0x08);

	init_keypad();

//	display_from_right(1234);

	SystemClock_Config_Interrupt(10, 10000);

	EXTI_Setup();

	while(1){}

	return 0;
}
#endif

#ifdef LAB3
#define waitState 0
#define triggerStete 1
#define runState 2

int now_num = 0;
int state = waitState;


void EXTI_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13_Msk;
	SYSCFG->EXTICR[3] |= (2 << SYSCFG_EXTICR4_EXTI13_Pos); // PC13

	EXTI->IMR1 |= EXTI_IMR1_IM13;

	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler()
{
	if(EXTI->PR1 & EXTI_PR1_PIF13_Msk)
	{
//		set_gpio(LED_gpio, LED_pin);
		if(state == waitState)
		{
			state = runState;
			SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);
		}
		else if(state == triggerStete)
		{
			state = waitState;
			reset_gpio(LED_gpio, LED_pin);
			timer_stop(COUNTER_timer);
		}
		EXTI->PR1 = EXTI_PR1_PIF13_Msk;
	}
}

void SysTick_Handler()
{
	if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
	{
		display_from_right(now_num);
		now_num--;
	}
}

int main()
{
	FPU_init();

	GPIO_init_AF();
	timer_enable(COUNTER_timer);
	PWM_channel_init();
	timer_init(COUNTER_timer, 200000 / 4186, 100);

	EXTI_Setup();

	init_led(LED_gpio, LED_pin);
	init_button(BUTTON_gpio, BUTTON_pin);

	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Decode(0x00);
	set_ScanLimit(0x07);
	set_Shutdown(0x01);
	set_Intensity(0x08);

	init_keypad();

	SystemClock_Config_Interrupt(10, 10000000);
	SysTick->CTRL &= ~(1 << SysTick_CTRL_ENABLE_Pos);

	int button_press_cycle_per_second = 20;
	int debounce_cycles = 50;
	int debounce_threshold = debounce_cycles*0.7;
	int last_button_state = 0;

	// Input one number from keypad
	int k = 0;
	int flag_debounce = 0;
	int flag_keypad = 0;
	int flag_keypad_r = 0;
	int position_c = 0;
	int position_r = 0;
	int key_table[4][4] = {{1, 2, 3, 10},
							{4, 5, 6, 11},
							{7, 8, 9, 12},
							{15, 0, 14, 13}};
	int rst = 0;

	while(1)
	{
		flag_keypad = GPIOB->IDR & 0b10111 << 5;
		if(flag_keypad != 0)
		{
			k = 4500;
		}
		while(k != 0)
		{
			flag_debounce = GPIOB->IDR & 0b10111 << 5;
			k--;
		}
		if(flag_debounce != 0)
		{
			for(int i=0; i<4; i++)
			{
				position_c = i+8;
				// Because we use the port 8,9,10,12
				if(i==3)
					position_c++;
				// Set the output of the GPIOA
				GPIOA->ODR = (GPIOA->ODR & 0xFFFFE8FF) | 1 << position_c;
				for(int j=0; j<4; j++)
				{
					position_r = j+5;
					if(j==3)
						position_r++;
					flag_keypad_r = GPIOB->IDR & 1 << position_r;
					if(flag_keypad_r != 0)
					{
						rst = key_table[j][i];
						display_from_right(rst);
						now_num = rst;
					}
				}
			}
		}

		if(now_num == -1)
		{
			if(state == waitState)
			{
				display_from_right(0);
				SysTick->CTRL &= ~(1 << SysTick_CTRL_ENABLE_Pos);
			}
			else if(state == runState)
			{
				state = triggerStete;
				SysTick->CTRL &= ~(1 << SysTick_CTRL_ENABLE_Pos);
				set_gpio(LED_gpio, LED_pin);
				timer_init(COUNTER_timer, 200000 / 4186, 100);
				timer_start(COUNTER_timer);
			}
		}
	}

	while(1){}

	return 0;
}
#endif
