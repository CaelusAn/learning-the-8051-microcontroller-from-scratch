/*
 * File:   Timer0.c
 * Brief:  Timer0 initialization for the LED streaming project.
 * Author: CaelusAn
 * Date:   2026-10-18
 */

#include <REGX52.H>

/*
 * @brief  Initialize Timer0 in 16-bit mode with a 1 ms interrupt interval.
 * @param  None
 * @retval None
 * @note   The reload values assume a 12 MHz clock and 12-clock machine cycle.
 */
void Timer0_Init(void)
{
    /* Select Timer0 mode 1 and preserve the Timer1 configuration bits. */
    TMOD = TMOD & 0xF0;
    TMOD = TMOD | 0x01;

    /* Clear the overflow flag and start Timer0. */
    TF0 = 0;
    TR0 = 1;

    /* Reload values for approximately 1 ms at 12 MHz. */
    TL0 = 0x18;
    TH0 = 0xFC;

    /* Enable Timer0 and global interrupts with low interrupt priority. */
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

/*
 * Timer0_Routine() is implemented in main.c in this learning snapshot.
 */
