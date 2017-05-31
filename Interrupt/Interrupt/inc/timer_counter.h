#include "stm32l476xx.h"

void timer_enable(TIM_TypeDef* timer);

void timer_init(TIM_TypeDef* timer, int psc, int arr);

void timer_start(TIM_TypeDef* timer);

void timer_stop(TIM_TypeDef* timer);

void SystemClock_Config(int speed);

void SystemClock_Config_Interrupt(int speed, int load);

void GPIO_init_AF();

void PWM_channel_init();
