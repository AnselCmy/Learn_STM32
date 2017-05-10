#include "stm32l476xx.h"

void timer_enable(TIM_TypeDef* timer);

void timer_init(TIM_TypeDef* timer, int psc, int arr);

void timer_start(TIM_TypeDef* timer);

void timer_stop(TIM_TypeDef* timer);
