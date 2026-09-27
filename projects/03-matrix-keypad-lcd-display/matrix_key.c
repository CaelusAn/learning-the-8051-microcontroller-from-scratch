/*
 * File:   matrix_key.c
 * Brief:  4x4 matrix keypad driver for the PuZhong 8051 board.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "matrix_key.h"

#define MATRIX_KEY_ROW_COUNT        4U
#define MATRIX_KEY_COLUMN_COUNT     4U
#define MATRIX_KEY_ALL_PINS_HIGH    0xFFU
#define MATRIX_KEY_COLUMN_LOW_BIT   0x01U
#define MATRIX_KEY_ROW_READ_HIGH_BIT 0x80U
#define MATRIX_KEY_DEBOUNCE_MS      20U

/*
 * @brief  Scan the matrix keypad and return the pressed key number.
 * @param  None
 * @retval Key number from 1 to 16, or MATRIX_KEY_NONE when no key is pressed.
 */
u8 MatrixKey(void)
{
    static const u8 keyMap[MATRIX_KEY_COLUMN_COUNT][MATRIX_KEY_ROW_COUNT] =
    {
        { 4U,  8U, 12U, 16U },
        { 3U,  7U, 11U, 15U },
        { 2U,  6U, 10U, 14U },
        { 1U,  5U,  9U, 13U }
    };
    u8 columnIndex;
    u8 rowIndex;
    u8 rowMask;

    for (columnIndex = 0U; columnIndex < MATRIX_KEY_COLUMN_COUNT; columnIndex++)
    {
        P1 = MATRIX_KEY_ALL_PINS_HIGH;
        P1 &= ~(MATRIX_KEY_COLUMN_LOW_BIT << columnIndex);

        for (rowIndex = 0U; rowIndex < MATRIX_KEY_ROW_COUNT; rowIndex++)
        {
            rowMask = (u8)(MATRIX_KEY_ROW_READ_HIGH_BIT >> rowIndex);

            if ((P1 & rowMask) == 0U)
            {
                DelayMs(MATRIX_KEY_DEBOUNCE_MS);

                if ((P1 & rowMask) == 0U)
                {
                    while ((P1 & rowMask) == 0U)
                    {
                    }

                    DelayMs(MATRIX_KEY_DEBOUNCE_MS);
                    return keyMap[columnIndex][rowIndex];
                }
            }
        }
    }

    return MATRIX_KEY_NONE;
}
