/*
 * File:   lcd1602.h
 * Brief:  LCD1602 display driver interface.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "public.h"

/*
 * Select the LCD data interface width.
 * 0: 8-bit data interface
 * 1: 4-bit data interface
 */
#define LCD1602_4OR8_DATA_INTERFACE 0U

#define LCD1602_ROW_COUNT    2U
#define LCD1602_COLUMN_COUNT 16U
#define LCD1602_ROW_1        0U
#define LCD1602_ROW_2        1U
#define LCD1602_COLUMN_START 0U

/* LCD control pins */
sbit LCD1602_RS = P2^6; /* Register select: 0 for command, 1 for data */
sbit LCD1602_RW = P2^5; /* Read/write select: 0 for write, 1 for read */
sbit LCD1602_E  = P2^7; /* Enable signal */

#define LCD1602_DATA_PORT P0

void LCD1602_Init(void);
void LCD1602_Clear(void);
void LCD1602_ShowString(u8 column, u8 row, const char *text);

#endif
