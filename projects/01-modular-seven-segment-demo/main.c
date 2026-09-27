/*
 * File:   main.c
 * Brief:  Blink an LED on P2.0 as a basic timing demonstration.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "public.h"

#define BLINK_LED         P2_0
#define BLINK_INTERVAL_MS 500U

void main(void)
{
    while (1)
    {
        BLINK_LED = 1;
        DelayMs(BLINK_INTERVAL_MS);
        BLINK_LED = 0;
        DelayMs(BLINK_INTERVAL_MS);
    }
}
