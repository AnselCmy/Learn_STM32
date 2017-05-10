#include <7seg.h>
#include <gpio.h>
#include "stm32l476xx.h"

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
void set_Decode(uint8_t d)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DECODE_MODE, d);
}

/*
 * Scan limit from 0x00 to 0x07
 */
void set_ScanLimit(uint8_t s)
{
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_SCAN_LIMIT, s);
}

uint8_t dot_Point(uint8_t n)
{
	return  (0x80 + n);
}

void clear_all()
{
	for(int i=1; i<=8; i++)
	{
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, i, 0);
	}
}

int get_Non_Decode(int num)
{
	int data = 0;
	switch(num)
	{
		case 0:
			data = SEG_DATA_NON_DECODE_0; break;
		case 1:
			data = SEG_DATA_NON_DECODE_1; break;
		case 2:
			data = SEG_DATA_NON_DECODE_2; break;
		case 3:
			data = SEG_DATA_NON_DECODE_3; break;
		case 4:
			data = SEG_DATA_NON_DECODE_4; break;
		case 5:
			data = SEG_DATA_NON_DECODE_5; break;
		case 6:
			data = SEG_DATA_NON_DECODE_6; break;
		case 7:
			data = SEG_DATA_NON_DECODE_7; break;
		case 8:
			data = SEG_DATA_NON_DECODE_8; break;
		case 9:
			data = SEG_DATA_NON_DECODE_9; break;
	}
	return data;
}

void display_one(int loc, int num, int decode_flag)
{
	int address = 0;
	switch(loc)
	{
		case 0:
			address = SEG_ADDRESS_DIGIT_0; break;
		case 1:
			address = SEG_ADDRESS_DIGIT_1; break;
		case 2:
			address = SEG_ADDRESS_DIGIT_2; break;
		case 3:
			address = SEG_ADDRESS_DIGIT_3; break;
		case 4:
			address = SEG_ADDRESS_DIGIT_4; break;
		case 5:
			address = SEG_ADDRESS_DIGIT_5; break;
		case 6:
			address = SEG_ADDRESS_DIGIT_6; break;
		case 7:
			address = SEG_ADDRESS_DIGIT_7; break;
		default:
			address = SEG_ADDRESS_DIGIT_0; break;
	}

	int data = num;
	if(decode_flag == NotDecode)
	{
		switch(num)
		{
			case 0:
				data = SEG_DATA_NON_DECODE_0; break;
			case 1:
				data = SEG_DATA_NON_DECODE_1; break;
			case 2:
				data = SEG_DATA_NON_DECODE_2; break;
			case 3:
				data = SEG_DATA_NON_DECODE_3; break;
			case 4:
				data = SEG_DATA_NON_DECODE_4; break;
			case 5:
				data = SEG_DATA_NON_DECODE_5; break;
			case 6:
				data = SEG_DATA_NON_DECODE_6; break;
			case 7:
				data = SEG_DATA_NON_DECODE_7; break;
			case 8:
				data = SEG_DATA_NON_DECODE_8; break;
			case 9:
				data = SEG_DATA_NON_DECODE_9; break;
		}
	}
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, address, data);
}

void display_from_right(int number)
{
	clear_all();
	int neg = 0;
	if(number < 0)
	{
		number = -number; // Dealing the positive first
		neg = 1;
	}
	int cnt = 0;
	int num_arr[8];
	int remain = number % 10;
	int num = number / 10;
	int len = 0;
	num_arr[cnt] = remain;
	while(num)
	{
		cnt++;
		remain = num % 10;
		num = num / 10;
		num_arr[cnt] = remain;
	}
	len = cnt+1;
	// Display
	for(int i=0; i<=cnt; i++)
	{
		display_one(i, num_arr[i], NotDecode);
	}
	if(neg)
		send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, len+1, SEG_DATA_NON_DECODE_DASH);
}

void display_two_decimal(double num)
{
	int dec_1 = (int)(num*10)%10;
	int dec_2 = (int)(num*100)%10;
	display_one(0, dec_2, NotDecode);
	display_one(1, dec_1, NotDecode);
	int number = num;
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, 3, dot_Point(get_Non_Decode(number%10)));
	number = number/10;
	for(int i=3; i<8; i++)
	{
		if(number > 0)
		{
			display_one(i, number%10, NotDecode);
			number /= 10;
		}
		else
		{
			send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, i+1, SEG_DATA_NON_DECODE_BLANK);
		}
	}
	return;
}
