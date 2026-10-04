/*
 * File:   timer.h
 * Brief:  Timer0 millisecond and second time base interface.
 * Author: CaelusAn
 * Date:   2026-10-04
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "public.h"

void Timer0_Init(void);
u16 Timer0_GetElapsedSeconds(void);

#endif
