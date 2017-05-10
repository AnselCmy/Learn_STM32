#include <keypad.h>
#include <stm32l476xx.h>
#include <gpio.h>
#include <7seg.h>

unsigned int x_pin[] = {x0, x1, x2, x3};
unsigned int y_pin[] = {y0, y1, y2, y3};

void init_keypad()
{
	// Set GPIOA as output
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	for(int p=0; p<4; p++)
	{
		set_gpio(GPIOA, x_pin[p]);
	}
	GPIOA->MODER = GPIOA->MODER & 0xFDD5FFFF;
	GPIOA->PUPDR = GPIOA->PUPDR | 0x1150000;
	GPIOA->OSPEEDR = GPIOA->OSPEEDR | 0x115000;
	GPIOA->ODR = GPIOA->ODR | 0b10111 << 8;

	// Set GPIOB as output
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	for(int p=0; p<4; p++)
	{
		set_gpio(GPIOB, y_pin[p]);
	}
	GPIOB->MODER = GPIOB->MODER & 0xFFF303FF;
	GPIOB->PUPDR = GPIOB->PUPDR | 0x8A800;
	GPIOB->OSPEEDR = GPIOB->OSPEEDR | 0x45400;
}
