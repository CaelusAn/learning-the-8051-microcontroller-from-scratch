/*
 * File:   timer.c
 * Brief:  Timer0 time base for password lock delays and countdowns.
 * Author: CaelusAn
 * Date:   2026-10-04
 */

#include "timer.h"

/*
 * The source assumes a 12 MHz crystal and the traditional 12-clock 8051
 * machine cycle. Change TIMER0_CPU_CLOCK_HZ when the board clock is different.
 */
#define TIMER0_CPU_CLOCK_HZ       12000000UL
#define TIMER0_MACHINE_CYCLES      12UL
#define TIMER0_TICK_HZ             100UL
#define TIMER0_TICKS_PER_SECOND    100U
#define TIMER0_RELOAD_VALUE        (65536UL - \
                                    (TIMER0_CPU_CLOCK_HZ / \
                                     TIMER0_MACHINE_CYCLES / \
                                     TIMER0_TICK_HZ))

static volatile u8 timerSubTicks = 0U;
static volatile u16 timerElapsedSeconds = 0U;

/*
 * @brief  Stop or restore Timer0 interrupts while reading the shared time.
 * @param  enable Nonzero value enables Timer0 interrupts.
 * @retval None
 */
static void Timer0_SetInterruptEnable(u8 enable)
{
    if (enable != 0U)
    {
        ET0 = 1;
    }
    else
    {
        ET0 = 0;
    }
}

/*
 * @brief  Initialize Timer0 as a 10 ms interval timer.
 * @param  None
 * @retval None
 */
void Timer0_Init(void)
{
    TMOD = (TMOD & 0xF0U) | 0x01U;
    TH0 = (u8)(TIMER0_RELOAD_VALUE >> 8U);
    TL0 = (u8)TIMER0_RELOAD_VALUE;

    timerSubTicks = 0U;
    timerElapsedSeconds = 0U;

    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

/*
 * @brief  Return the number of seconds elapsed since Timer0 initialization.
 * @param  None
 * @retval Elapsed seconds. The 16-bit value wraps after about 18 hours.
 */
u16 Timer0_GetElapsedSeconds(void)
{
    u16 elapsedSeconds;
    u8 timerInterruptWasEnabled;

    timerInterruptWasEnabled = ET0;
    Timer0_SetInterruptEnable(0U);

    elapsedSeconds = timerElapsedSeconds;

    Timer0_SetInterruptEnable(timerInterruptWasEnabled);

    return elapsedSeconds;
}

/*
 * @brief  Timer0 interrupt service routine.
 * @param  None
 * @retval None
 */
void Timer0_Isr(void) interrupt 1
{
    TH0 = (u8)(TIMER0_RELOAD_VALUE >> 8U);
    TL0 = (u8)TIMER0_RELOAD_VALUE;

    timerSubTicks++;
    if (timerSubTicks >= TIMER0_TICKS_PER_SECOND)
    {
        timerSubTicks = 0U;
        timerElapsedSeconds++;
    }
}
