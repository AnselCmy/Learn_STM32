#include "stm32l476xx.h"

// Define a lot of Non-Decode Mode Constants
#define SEG_DATA_NON_DECODE_0		0b1111110
#define SEG_DATA_NON_DECODE_1		0b0110000
#define SEG_DATA_NON_DECODE_2		0b1101101
#define SEG_DATA_NON_DECODE_3		0b1111001
#define SEG_DATA_NON_DECODE_4		0b0110011
#define SEG_DATA_NON_DECODE_5		0b1011011
#define SEG_DATA_NON_DECODE_6		0b1011111
#define SEG_DATA_NON_DECODE_7		0b1110000
#define SEG_DATA_NON_DECODE_8		0b1111111
#define SEG_DATA_NON_DECODE_9		0b1111011
#define SEG_DATA_NON_DECODE_A		0b1110111
#define SEG_DATA_NON_DECODE_B		0b0011111
#define SEG_DATA_NON_DECODE_C		0b1001110
#define SEG_DATA_NON_DECODE_D		0b0111101
#define SEG_DATA_NON_DECODE_E		0b1001111
#define SEG_DATA_NON_DECODE_F		0b1000111
#define SEG_DATA_NON_DECODE_DASH	0b0000001
#define SEG_DATA_NON_DECODE_BLANK	0b0000000

// Define ADDRESS Constants for 7seg commands
#define SEG_ADDRESS_NOP				0
#define SEG_ADDRESS_DIGIT_0			1
#define SEG_ADDRESS_DIGIT_1			2
#define SEG_ADDRESS_DIGIT_2			3
#define SEG_ADDRESS_DIGIT_3			4
#define SEG_ADDRESS_DIGIT_4			5
#define SEG_ADDRESS_DIGIT_5			6
#define SEG_ADDRESS_DIGIT_6			7
#define SEG_ADDRESS_DIGIT_7			8
#define SEG_ADDRESS_DECODE_MODE		9
#define SEG_ADDRESS_INTENSITY		10
#define SEG_ADDRESS_SCAN_LIMIT		11
#define SEG_ADDRESS_SHUTDOWN		12
#define SEG_ADDRESS_DISPLAY_TEST	15

// Define DATA Constants for 7seg commands
#define SEG_DATA_DECODE_0		0
#define SEG_DATA_DECODE_1		1
#define SEG_DATA_DECODE_2		2
#define SEG_DATA_DECODE_3		3
#define SEG_DATA_DECODE_4		4
#define SEG_DATA_DECODE_5		5
#define SEG_DATA_DECODE_6		6
#define SEG_DATA_DECODE_7		7
#define SEG_DATA_DECODE_8		8
#define SEG_DATA_DECODE_9		9
#define SEG_DATA_DECODE_DASH	10
#define SEG_DATA_DECODE_E		11
#define SEG_DATA_DECODE_H		12
#define SEG_DATA_DECODE_L		13
#define SEG_DATA_DECODE_P		14
#define SEG_DATA_DECODE_BLANK	15

// Define the flag for encode
#define Decode 1
#define NotDecode 0

// Define gpio port and pin
#define SEG_gpio	GPIOB
#define CLK_pin			3
#define DIN_pin			4
#define CS_pin			8

int init_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLk);

void send_7seg(GPIO_TypeDef* gpio, int DIN, int CS, int CLk, int address, int data);

void set_Intensity(uint8_t i);

void set_Shutdown(uint8_t s);

void set_Decode(uint8_t d);

void set_ScanLimit(uint8_t s);

uint8_t dot_Point(uint8_t n);

void clear_all();

void display_one(int loc, int num, int decode_flag);

void display_from_right(int number);

void display_two_decimal(double number);
