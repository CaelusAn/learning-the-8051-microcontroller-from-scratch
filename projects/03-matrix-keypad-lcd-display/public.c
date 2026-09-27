#include "public.h"

/*
 * Function: delay_10us
 * Description: Approximate delay loop based on units of 10 microseconds.
 * Parameter: ten_us - Number of 10-microsecond units to delay.
 */
void delay_10us(u16 ten_us)
{
	while(ten_us--);
}

/*
 * Function: delay_ms
 * Description: Approximate millisecond delay. The actual timing depends on
 *              the microcontroller clock frequency and compiler settings.
 * Parameter: ms - Number of milliseconds to delay.
 */
void delay_ms(u16 ms)
{
	u16 i,j;
	for(i=ms;i>0;i--)
		for(j=110;j>0;j--);
}
