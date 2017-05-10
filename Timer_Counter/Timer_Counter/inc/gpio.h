#include "stm32l476xx.h"

void set_gpio(GPIO_TypeDef* gpio, int pin);

void reset_gpio(GPIO_TypeDef* gpio, int pin);

uint32_t read_gpio(GPIO_TypeDef* gpio, int pin);
