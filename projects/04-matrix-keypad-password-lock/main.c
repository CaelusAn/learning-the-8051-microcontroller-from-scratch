/*
 * File:   main.c
 * Brief:  Entry point for the six-digit matrix keypad password lock.
 * Author: CaelusAn
 * Date:   2026-10-04
 */

#include "lcd1602.h"
#include "password_lock.h"
#include "timer.h"

void main(void)
{
    LCD1602_Init();
    Timer0_Init();
    PasswordLock_Init();

    while (1)
    {
        PasswordLock_Update();
    }
}
