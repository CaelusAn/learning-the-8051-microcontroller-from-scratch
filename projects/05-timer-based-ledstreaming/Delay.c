/*
 * File:   Delay.c
 * Brief:  Software millisecond delay for the LED streaming project.
 * Author: CaelusAn
 * Date:   2026-10-18
 */

#include <REGX52.H>

/*
 * @brief  Delay for approximately the requested number of milliseconds.
 * @param  tms Number of milliseconds to delay.
 * @retval None
 * @note   The loop is calibrated for a 12 MHz clock and is approximate.
 */
void Delayms(unsigned int tms)
{
    unsigned char i, j;

    while(tms--)
    {
        i = 2;
        j = 239;
        do
        {
            while(--j);
        }while(--i);
    }
}
