/*
 * File:   main.c
 * Brief:  Timer0-based LED streaming demonstration.
 * Author: CaelusAn
 * Date:   2026-10-18
 */

#include <REGX52.H>
#include "Timer0.h"
#include "Key.h"
#include <INTRINS.H>

/* Latest key returned by Key(). Zero means no key was pressed. */
unsigned char KeyNum;

/* LED direction: 0 shifts left, 1 shifts right. */
unsigned char LEDMode;

void main()
{
    /* Active-low LEDs: one zero bit moves through P2. */
    P2 = 0xFE;
    Timer0_Init();

    while(1)
    {
        KeyNum = Key();
        if(KeyNum)
        {
            /* Key 1 toggles the streaming direction. */
            if(KeyNum == 1)
            {
                LEDMode++;
                if(LEDMode >= 2)
                    LEDMode = 0;
            }
        }
    }
}

/*
 * @brief  Timer0 interrupt service routine.
 * @param  None
 * @retval None
 * @note   Timer0 expires every 1 ms. One hundred interrupts produce a
 *         100 ms LED update interval.
 */
void Timer0_Routine() interrupt 1
{
    static unsigned int T0Count;

    /* Reload the 1 ms counter value for a 12 MHz clock. */
    TL0 = 0x18;
    TH0 = 0xFC;

    T0Count++;
    if(T0Count >= 100)
    {
        T0Count = 0;

        /* Shift the active-low LED pattern in the selected direction. */
        if(LEDMode == 0)
            P2 = _crol_(P2, 1);
        if(LEDMode == 1)
            P2 = _cror_(P2, 1);
    }
}
