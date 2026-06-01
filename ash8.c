#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define COLS 40
#define ROWS 25
#define SCALE 16

int main(void)
{
	uint8_t screen[COLS * ROWS] = {0};

	screen[0] = 'A';

	printf("screen[0] contain: %c\n", screen[0]);

	return 0 ;
}
