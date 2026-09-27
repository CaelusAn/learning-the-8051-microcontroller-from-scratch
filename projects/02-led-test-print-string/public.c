/*
 * File:   public.c
 * Brief:  Common delay routines for 8051 projects.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "public.h"

#define DELAY_MS_INNER_LOOPS 110U

/*
 * @brief  Delay for approximately the requested number of 10 us units.
 * @param  delayUnits Number of 10 us units to delay.
 * @retval None
 */
void Delay10us(u16 delayUnits)
{
    while (delayUnits > 0U)
    {
        delayUnits--;
    }
}

/*
 * @brief  Delay for approximately the requested number of milliseconds.
 * @param  milliseconds Number of milliseconds to delay.
 * @retval None
 * @note   The timing is calibrated for a 12 MHz clock and may vary with the
 *         compiler and optimization settings.
 */
void DelayMs(u16 milliseconds)
{
    u16 outerIndex;
    u16 innerIndex;

    for (outerIndex = milliseconds; outerIndex > 0U; outerIndex--)
    {
        for (innerIndex = DELAY_MS_INNER_LOOPS; innerIndex > 0U; innerIndex--)
        {
        }
    }
}
