#include "led_button.h"

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
	gpio->MODER |= (0b00 << (2*pin));
	return 0;
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
