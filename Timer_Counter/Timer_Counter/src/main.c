#include <stm32l476xx.h>
#include <keypad.h>
#include <7seg.h>
#include <gpio.h>
#include <timer_counter.h>
#include "stdlib.h"

//#define LAB1
//#define LAB2
#define LAB3

#define LED_gpio GPIOA
#define LED_pin 5

#define BUTTON_gpio GPIOC
#define BUTTON_pin 13

#define COUNTER_timer TIM2

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

int init_led(GPIO_TypeDef* gpio, int pin)
{
	if(gpio == GPIOA)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	}
	else if(gpio == GPIOB)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	}
	else
	{
		return -1;
	}

	gpio->MODER &= ~(0b11 << (2*pin));
	gpio->MODER |= (0b01 << (2*pin));// output mode
	return 0;
}

int init_button(GPIO_TypeDef* gpio, int pin)
{
	if(gpio == GPIOC)
	{
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	}
	else
	{
		return -1;
	}

	gpio->MODER &= ~(0b11 << (2*pin));
	gpio->MODER |= (0b00 << (2*pin));// output mode
	return 0;
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
	// 8<=N<=86, R=2,4,6,8, 1<=M<=8
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

#ifdef LAB1
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

	int speed = 0;
	int trans[5] = {1, 6, 10, 16, 40};
	SystemClock_Config(trans[speed]);

	// The state of LED
	int state = 0;
	int button_press_cycle_per_second = 10;
	int debounce_cycles = 100;
	int debounce_threshold = debounce_cycles * 0.7;
	// 0-not pressed
	int last_button_state = 0;

//	while(1)
//	{
//		if(read_gpio(BUTTON_gpio, BUTTON_pin)==0)
//		{
//			set_gpio(LED_gpio, LED_pin);
//		}
//		reset_gpio(LED_gpio, LED_pin);
//	}

	while(1)
	{
		// Do that ten times every 1 sec
		for(int a=0; a<button_press_cycle_per_second; a++)
		{
			int pos_cnt = 0;
			// Check 100 times in one
			for(int a=0; a<debounce_cycles; a++)
			{
				if(read_gpio(BUTTON_gpio, BUTTON_pin)==1)
				{
					pos_cnt++;
				}
				delay_without_interrupt(1000/(button_press_cycle_per_second*debounce_cycles));
			}

			if(pos_cnt>debounce_threshold)
			{
				last_button_state = 1;
			}
			else
			{
				if(last_button_state == 1)
				{
					speed = (speed+1)%5;
					SystemClock_Config(trans[speed]);
				}
				last_button_state = 0;
			}
		}
		if(state == 1)
		{
			reset_gpio(LED_gpio, LED_pin);
		}
		else
		{
			set_gpio(LED_gpio, LED_pin);
		}
		state = 1-state;
	}
	while(1){}
	return 0;
}
#endif


#ifdef LAB2
int main()
{
	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Decode(0x00);
	set_ScanLimit(0x07);
	set_Shutdown(0x01);
	set_Intensity(0x08);

	FPU_init();


	double TIME_SEC = 12.7;
	if(TIME_SEC < 0.0 || TIME_SEC > 10000.0)
	{
		display_two_decimal(0.0);
	}
	else
	{
		timer_enable(COUNTER_timer);
		timer_init(COUNTER_timer, 40000, 100);
		timer_start(COUNTER_timer);
		int sec = 0, last = 0;
		while(1)
		{
			if(last!=COUNTER_timer->CNT)
			{
				if(COUNTER_timer->CNT == 0)
					sec++;
				last = COUNTER_timer->CNT;
				double now_time = sec + COUNTER_timer->CNT/100.0;
				display_two_decimal(now_time);
				if(now_time == TIME_SEC)
				{
					break;
				}
			}
		}
		timer_stop(COUNTER_timer);
	}
}
#endif

#ifdef LAB3
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

int main()
{
	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Decode(0x00);
	set_ScanLimit(0x07);
	set_Shutdown(0x01);
	set_Intensity(0x08);

	FPU_init();
	GPIO_init_AF();
	timer_enable(COUNTER_timer);
	PWM_channel_init();

	init_keypad();
	int realGet = 0;
	int flag_keypad = 0;
	int flag_debounce = 0;
	int position_c = 0;
	int position_r = 0;
	int k = 0;
	int flag_keypad_r;
	char key_table[4][4] = {{'1', '2', '3', 'A'},
							{'4', '5', '6', 'B'},
							{'7', '8', '9', 'C'},
							{'*', '0', '#', 'D'}};
	int get_key = 0;

//	timer_init(COUNTER_timer, 200000 / 1175, 100);
//	timer_start(COUNTER_timer);

	char key_arr[] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
	int voice[] = {4186, 4435, 4699, 4978, 5274, 5587, 5919, 6271, 6645, 7040, 7459, 7902};
	display_from_right(101);

	while(1)
	{
//		for(int i=1; i< 100000; i++)
//		{;}
//		realGet = 0;
		get_key = 0;
		flag_keypad = GPIOB->IDR & 0b10111 << 5;
		if(flag_keypad != 0)
		{
			k = 45000;
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
						get_key = j*4 + i;
						display_from_right(get_key);
						timer_init(COUNTER_timer, 200000 / voice[get_key], 100);
						timer_start(COUNTER_timer);
						for(int i=0; i<100000; i++)
						{;}
						timer_stop(COUNTER_timer);
					}
				}
			}
		}
	}
}
#endif
