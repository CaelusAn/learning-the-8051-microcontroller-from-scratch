#include <REGX52.H>
#include "Delayms.h"
#include "lcd1602.h"
#include "matrix_key.h"

/*
 * Convert a number (0~99) to a two-digit string with leading zero.
 * Example: 1 -> "01", 12 -> "12"
 */
static void NumToStr2(u8 num, u8 *str)
{
    str[0] = num / 10 + '0';
    str[1] = num % 10 + '0';
    str[2] = '\0';
}

void main(void)
{
    u8 key;
    u8 str[3];

    lcd1602_init();
    lcd1602_show_string(0, 0, "Matrix Key:");

    while (1)
    {
        key = MatrixKey();

        if (key != 0)
        {
            NumToStr2(key, str);
            lcd1602_show_string(0, 1, str);   /* Show on second line */
        }
    }
}