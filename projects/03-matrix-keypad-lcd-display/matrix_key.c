/* matrix_key.c */
#include <REGX52.H>
#include "Delayms.h"

/*
 * Matrix keypad scanning for PuZhong 8051 board.
 * Rows:    P1.0 ~ P1.3  (output low to scan)
 * Columns: P1.4 ~ P1.7  (input, low when pressed)
 *
 * Key map:
 *   P1.3:  1  5  9 13
 *   P1.2:  2  6 10 14
 *   P1.1:  3  7 11 15
 *   P1.0:  4  8 12 16
 */
unsigned char MatrixKey(void)
{
    unsigned char row, col;
    unsigned char key_map[4][4] = {
        { 4,  8, 12, 16},   /* P1.0 */
        { 3,  7, 11, 15},   /* P1.1 */
        { 2,  6, 10, 14},   /* P1.2 */
        { 1,  5,  9, 13}    /* P1.3 */
    };

    for (row = 0; row < 4; row++)
    {
        P1 = 0xFF;                  /* All rows high */
        P1 &= ~(0x01 << row);       /* Pull only current row low */

        for (col = 0; col < 4; col++)
        {
            /* Check if current column is low */
            if (!(P1 & (0x80 >> col)))
            {
                Delayms(20);        /* Debounce press */

                if (!(P1 & (0x80 >> col)))
                {
                    /* Wait until key is released */
                    while (!(P1 & (0x80 >> col)));

                    Delayms(20);    /* Debounce release */
                    return key_map[row][col];
                }
            }
        }
    }

    return 0;   /* No key pressed */
}