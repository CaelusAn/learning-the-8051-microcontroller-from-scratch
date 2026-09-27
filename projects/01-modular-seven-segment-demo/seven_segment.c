/*
 * File:   seven_segment.c
 * Brief:  Seven-segment display driver for the PuZhong 8051 board.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "seven_segment.h"

#define SEVEN_SEGMENT_POSITION_MASK 0x1CU
#define SEVEN_SEGMENT_REFRESH_MS    1U
#define SEVEN_SEGMENT_BLANK_OUTPUT  0x00U

static const u8 SEVEN_SEGMENT_DIGIT_TABLE[SEVEN_SEGMENT_DIGIT_COUNT] =
{
    0x3FU, 0x06U, 0x5BU, 0x4FU, 0x66U, 0x6DU, 0x7DU, 0x07U, 0x7FU,
    0x6FU, 0x77U, 0x7CU, 0x39U, 0x5EU, 0x79U, 0x71U, 0x00U
};

static const u8 SEVEN_SEGMENT_POSITION_TABLE[SEVEN_SEGMENT_POSITION_COUNT] =
{
    0x1CU, 0x18U, 0x14U, 0x10U, 0x0CU, 0x08U, 0x04U, 0x00U
};

/*
 * @brief  Display one digit at the selected seven-segment position.
 * @param  position Display position from 1 to 8.
 * @param  digit    Digit value from 0 to 16, where 16 is blank.
 * @retval None
 */
void SevenSegmentDisplay(u8 position, u8 digit)
{
    u8 positionValue;

    if ((position < 1U) || (position > SEVEN_SEGMENT_POSITION_COUNT))
    {
        return;
    }

    if (digit >= SEVEN_SEGMENT_DIGIT_COUNT)
    {
        return;
    }

    positionValue = SEVEN_SEGMENT_POSITION_TABLE[position - 1U];
    P2 = (P2 & ~SEVEN_SEGMENT_POSITION_MASK) | positionValue;
    P0 = SEVEN_SEGMENT_DIGIT_TABLE[digit];
    DelayMs(SEVEN_SEGMENT_REFRESH_MS);
    P0 = SEVEN_SEGMENT_BLANK_OUTPUT;
}
