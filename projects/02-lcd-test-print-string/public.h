/*
 * File:   public.h
 * Brief:  Common types and delay APIs for 8051 projects.
 * Author: CaelusAn
 * Date:   2026-09-27
 */

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include <REGX52.H>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

void Delay10us(u16 delayUnits);
void DelayMs(u16 milliseconds);

#endif
