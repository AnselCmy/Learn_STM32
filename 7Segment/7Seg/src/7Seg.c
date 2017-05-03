#include "stm32l476xx.h"
#include "7Seg.h"

int init_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLK)
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

	gpio->MODER &= ~(0b11 << (2*DIN));
	gpio->MODER |= (0b01 << (2*DIN));
	gpio->MODER &= ~(0b11 << (2*CS));
	gpio->MODER |= (0b01 << (2*CS));
	gpio->MODER &= ~(0b11 << (2*CLK));
	gpio->MODER |= (0b01 << (2*CLK));

	return 0;
}

void send_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLK, int address, int data)
{
	int payload = ((address & 0xFF) << 8) | (data & 0xFF);

	int total_cycles = 16+1;
	for(int a=1; a<=total_cycles; a++)
	{
		// Reset the CLK
		reset_gpio(gpio, CLK);

		if(a!=total_cycles && ((payload >> (total_cycles-1-a)) & 0x1))
		{
			set_gpio(gpio, DIN);
		}
		else
		{
			reset_gpio(gpio, DIN);
		}

		// Set the CS at last
		if(a == total_cycles)
		{
			set_gpio(gpio, CS);
		}
		else
		{
			reset_gpio(gpio, CS);
		}

		// Set data at raising edge
		set_gpio(gpio, CLK);
	}
	return;
}

/*
 * Set the intensity from 0x00 to x0f from dark to ligh
 */
void set_Intensity(uint8_t i)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_INTENSITY, i);
}

/*
 * 0x00 - shutdown mode
 * 0x01 - normal mode
 */
void set_Shutdown(uint8_t s)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SHUTDOWN, s);
}

/*
 * The param presents the address should be encoded, 0x00 all not encode, 0xff all encode
 */
void set_Encode(uint8_t d)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, d);
}

/*
 * Scan limit from 0x00 to 0x07
 */
void set_ScanLimit(uint8_t s)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, 0x00);
}

uint8_t dot_Point(uint8_t n)
{
	return  (0x80 + n);
}
