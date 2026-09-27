/*
 * File:   seven_segment.h
 * Brief:  Seven-segment display driver for the PuZhong 8051 board.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#ifndef __SEVEN_SEGMENT_H__
#define __SEVEN_SEGMENT_H__

#include "public.h"

#define SEVEN_SEGMENT_POSITION_COUNT 8U
#define SEVEN_SEGMENT_DIGIT_COUNT    17U

void SevenSegmentDisplay(u8 position, u8 digit);

#endif
