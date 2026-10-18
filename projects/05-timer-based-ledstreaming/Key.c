/*
 * File:   Key.c
 * Brief:  Debounced independent-key input for the PuZhong 8051 board.
 * Author: CaelusAn
 * Date:   2026-10-18
 */

#include <REGX52.H>
#include "Delay.h"

/*
 * @brief  Read one active-low independent key and wait for its release.
 * @param  None
 * @retval Key number from 1 to 4, or zero when no key is pressed.
 */
unsigned char Key()
{
    unsigned char KeyNumber = 0;

    /* P3.1 is reported as key 1. */
    if(P3_1 == 0)
    {
        Delayms(20);
        while(P3_1 == 0);
        Delayms(20);
        KeyNumber = 1;
    }

    /* P3.0 is reported as key 2. */
    if(P3_0 == 0)
    {
        Delayms(20);
        while(P3_0 == 0);
        Delayms(20);
        KeyNumber = 2;
    }

    /* P3.2 is reported as key 3. */
    if(P3_2 == 0)
    {
        Delayms(20);
        while(P3_2 == 0);
        Delayms(20);
        KeyNumber = 3;
    }

    /* P3.3 is reported as key 4. */
    if(P3_3 == 0)
    {
        Delayms(20);
        while(P3_3 == 0);
        Delayms(20);
        KeyNumber = 4;
    }

    return KeyNumber;
}
