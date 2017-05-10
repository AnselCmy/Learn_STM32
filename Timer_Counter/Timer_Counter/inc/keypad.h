#include <stm32l476xx.h>

// PA 8 9 10 12
#define PORT_COL GPIOA
#define x0 8
#define x1 9
#define x2 10
#define x3 12

// PB 5 6 7 9
#define PORT_ROW GPIOB
#define y0 5
#define y1 6
#define y2 7
#define y3 9

void init_keypad();

