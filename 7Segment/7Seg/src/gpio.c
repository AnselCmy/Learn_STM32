#include "gpio.h"

void set_gpio(GPIO_TypeDef* gpio, int pin)
{
	gpio->BSRR |= (1 << pin);
}

void reset_gpio(GPIO_TypeDef* gpio, int pin)
{
	gpio->BRR |= (1 << pin);
}
