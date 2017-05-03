#include "7Seg.h"
#include "stm32l476xx.h"

int main()
{
	init_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin);
	set_Encode(0x00);
	set_ScanLimit(0x00);
	set_Shutdown(0x01);
	set_Intensity(0x08);
	send_7seg(SEG_gpio, DIN_pin, CS_pin, CLK_pin, SEG_ADDRESS_DIGIT_0, dot_Point(SEG_DATA_NON_DECODE_2));
}
