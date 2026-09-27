/*
 * File:   main.c
 * Brief:  Display the pressed matrix keypad number on an LCD1602.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "public.h"
#include "lcd1602.h"
#include "matrix_key.h"

#define KEY_TEXT_LENGTH 3U

/*
 * @brief  Convert a number from 0 to 99 to a two-digit string.
 * @param  number Number to convert.
 * @param  text   Output buffer with room for two digits and a terminator.
 * @retval None
 */
static void NumberToTwoDigits(u8 number, u8 *text)
{
    text[0] = (u8)('0' + (number / 10U));
    text[1] = (u8)('0' + (number % 10U));
    text[2] = '\0';
}

void main(void)
{
    u8 keyValue;
    u8 keyText[KEY_TEXT_LENGTH];

    LCD1602_Init();
    LCD1602_ShowString(LCD1602_COLUMN_START, LCD1602_ROW_1, "Matrix Key:");

    while (1)
    {
        keyValue = MatrixKey();

        if (keyValue != MATRIX_KEY_NONE)
        {
            NumberToTwoDigits(keyValue, keyText);
            LCD1602_ShowString(LCD1602_COLUMN_START, LCD1602_ROW_2, keyText);
        }
    }
}
