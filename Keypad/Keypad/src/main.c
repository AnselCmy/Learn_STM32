#include <stm32l476xx.h>
#include <keypad.h>
#include <7seg.h>

int main()
{
	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Decode(0x00);
	set_ScanLimit(0x07);
	set_Shutdown(0x01);
	set_Intensity(0x08);

	init_keypad();

	display_from_right(-1);
	int k = 0;
	int flag_debounce = 0;
	int flag_keypad = 0;
	int flag_keypad_r = 0;
	int position_c = 0;
	int position_r = 0;
	char key_table[4][4] = {{'1', '2', '3', 'A'},
							{'4', '5', '6', 'B'},
							{'7', '8', '9', 'C'},
							{'*', '0', '#', 'D'}};
	int rst = 1;
	while(1)
	{
		flag_keypad = GPIOB->IDR & 0b10111 << 5;
		if(flag_keypad != 0)
		{
			k = 4500;
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
						rst = key_table[j][i] - 48;
						display_from_right(rst);
					}
				}
			}
		}
	}
	return 0;
}
