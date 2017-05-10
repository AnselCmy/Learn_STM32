#include "gpio.h"

void set_gpio(GPIO_TypeDef* gpio, int pin)
{
	gpio->BSRR |= (1 << pin);
}

void reset_gpio(GPIO_TypeDef* gpio, int pin)
{
	gpio->BRR |= (1 << pin);
}

uint32_t read_gpio(GPIO_TypeDef* gpio, int pin)
{
	return (gpio->IDR >> pin) & 0x1;
}
