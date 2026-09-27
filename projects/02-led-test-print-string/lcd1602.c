/*
 * File:   lcd1602.c
 * Brief:  LCD1602 command, data, and string display driver.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "lcd1602.h"

#define LCD1602_COMMAND_CLEAR_DISPLAY 0x01U
#define LCD1602_COMMAND_ENTRY_MODE    0x06U
#define LCD1602_COMMAND_DISPLAY_ON    0x0CU
#define LCD1602_COMMAND_SET_DDRAM     0x80U
#define LCD1602_SECOND_ROW_OFFSET     0x40U
#define LCD1602_ENABLE_DELAY_MS       1U

#if (LCD1602_4OR8_DATA_INTERFACE == 0U)
#define LCD1602_FUNCTION_SET 0x38U
#else
#define LCD1602_FUNCTION_SET 0x28U
#endif

/*
 * @brief  Write a command byte to the LCD1602.
 * @param  command LCD command byte.
 * @retval None
 */
static void LCD1602_WriteCommand(u8 command)
{
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_E = 0;

#if (LCD1602_4OR8_DATA_INTERFACE == 0U)
    LCD1602_DATA_PORT = command;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;
#else
    LCD1602_DATA_PORT = command & 0xF0U;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;

    LCD1602_DATA_PORT = (u8)(command << 4U);
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;
#endif
}

/*
 * @brief  Write a data byte to the LCD1602.
 * @param  value Character value to display.
 * @retval None
 */
static void LCD1602_WriteData(u8 value)
{
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_E = 0;

#if (LCD1602_4OR8_DATA_INTERFACE == 0U)
    LCD1602_DATA_PORT = value;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;
#else
    LCD1602_DATA_PORT = value & 0xF0U;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;

    LCD1602_DATA_PORT = (u8)(value << 4U);
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 1;
    DelayMs(LCD1602_ENABLE_DELAY_MS);
    LCD1602_E = 0;
#endif
}

/*
 * @brief  Initialize the LCD1602 for two-line text display.
 * @param  None
 * @retval None
 */
void LCD1602_Init(void)
{
    LCD1602_WriteCommand(LCD1602_FUNCTION_SET);
    LCD1602_WriteCommand(LCD1602_COMMAND_DISPLAY_ON);
    LCD1602_WriteCommand(LCD1602_COMMAND_ENTRY_MODE);
    LCD1602_WriteCommand(LCD1602_COMMAND_CLEAR_DISPLAY);
}

/*
 * @brief  Clear the LCD1602 display.
 * @param  None
 * @retval None
 */
void LCD1602_Clear(void)
{
    LCD1602_WriteCommand(LCD1602_COMMAND_CLEAR_DISPLAY);
}

/*
 * @brief  Calculate the LCD DDRAM address for the next character.
 * @param  column    Starting column from 0 to 15.
 * @param  row       Starting row: 0 for the first row, 1 for the second row.
 * @param  textIndex Zero-based character index in the string.
 * @retval LCD command byte containing the target DDRAM address.
 */
static u8 LCD1602_GetAddress(u8 column, u8 row, u8 textIndex)
{
    if (row == LCD1602_ROW_1)
    {
        if (textIndex < (LCD1602_COLUMN_COUNT - column))
        {
            return (u8)(LCD1602_COMMAND_SET_DDRAM + column + textIndex);
        }

        return (u8)(LCD1602_COMMAND_SET_DDRAM +
                    LCD1602_SECOND_ROW_OFFSET +
                    column +
                    textIndex -
                    LCD1602_COLUMN_COUNT);
    }

    if (textIndex < (LCD1602_COLUMN_COUNT - column))
    {
        return (u8)(LCD1602_COMMAND_SET_DDRAM +
                    LCD1602_SECOND_ROW_OFFSET +
                    column +
                    textIndex);
    }

    return (u8)(LCD1602_COMMAND_SET_DDRAM + column + textIndex -
                LCD1602_COLUMN_COUNT);
}

/*
 * @brief  Write a null-terminated string at the selected LCD position.
 * @param  column Starting column from 0 to 15.
 * @param  row    Starting row: 0 for the first row, 1 for the second row.
 * @param  text   Pointer to the null-terminated string.
 * @retval None
 * @note   Text wraps automatically between the two LCD rows.
 */
void LCD1602_ShowString(u8 column, u8 row, const char *text)
{
    u8 textIndex = 0U;

    if ((row >= LCD1602_ROW_COUNT) || (column >= LCD1602_COLUMN_COUNT))
    {
        return;
    }

    if (text == 0)
    {
        return;
    }

    while (text[textIndex] != '\0')
    {
        LCD1602_WriteCommand(LCD1602_GetAddress(column, row, textIndex));
        LCD1602_WriteData((u8)text[textIndex]);
        textIndex++;
    }
}
