#ifndef _lcd1602_H
#define _lcd1602_H

#include "public.h"

/*
 * Select the LCD data interface width.
 * 0: 8-bit data interface
 * 1: 4-bit data interface
 */
#define LCD1602_4OR8_DATA_INTERFACE	0

// LCD control pins
sbit LCD1602_RS=P2^6; // Register select: 0 for command, 1 for data
sbit LCD1602_RW=P2^5; // Read/write select: 0 for write, 1 for read
sbit LCD1602_E=P2^7;  // Enable signal
#define LCD1602_DATAPORT P0 // LCD data bus

// LCD1602 public functions
void lcd1602_init(void);
void lcd1602_clear(void);
void lcd1602_show_string(u8 x,u8 y,u8 *str);

#endif
