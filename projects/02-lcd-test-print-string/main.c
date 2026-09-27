/*
 * File:   main.c
 * Brief:  Display two demo strings on an LCD1602.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#include "lcd1602.h"

void main(void)
{
    LCD1602_Init();
    LCD1602_ShowString(LCD1602_COLUMN_START, LCD1602_ROW_1, "114514");
    LCD1602_ShowString(LCD1602_COLUMN_START, LCD1602_ROW_2, "1919810");

    while (1)
    {
    }
}
