#include "lcd1602.h"

/*
 * Function: lcd1602_write_cmd
 * Description: Write a command byte to the LCD1602.
 * Parameter: cmd - LCD command byte.
 */
#if (LCD1602_4OR8_DATA_INTERFACE==0) // 8-bit LCD interface
void lcd1602_write_cmd(u8 cmd)
{
	LCD1602_RS=0; // Select the command register
	LCD1602_RW=0; // Select write mode
	LCD1602_E=0;
	LCD1602_DATAPORT=cmd; // Place the command on the data bus
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the write cycle
}
#else // 4-bit LCD interface
void lcd1602_write_cmd(u8 cmd)
{
	LCD1602_RS=0; // Select the command register
	LCD1602_RW=0; // Select write mode
	LCD1602_E=0;
	LCD1602_DATAPORT=cmd; // Send the high nibble
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the high-nibble transfer

	LCD1602_DATAPORT=cmd<<4; // Send the low nibble
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the low-nibble transfer
}
#endif

/*
 * Function: lcd1602_write_data
 * Description: Write a data byte to the LCD1602.
 * Parameter: dat - Character data to display.
 */
#if (LCD1602_4OR8_DATA_INTERFACE==0) // 8-bit LCD interface
void lcd1602_write_data(u8 dat)
{
	LCD1602_RS=1; // Select the data register
	LCD1602_RW=0; // Select write mode
	LCD1602_E=0;
	LCD1602_DATAPORT=dat; // Place the data on the data bus
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the write cycle
}
#else // 4-bit LCD interface
void lcd1602_write_data(u8 dat)
{
	LCD1602_RS=1; // Select the data register
	LCD1602_RW=0; // Select write mode
	LCD1602_E=0;
	LCD1602_DATAPORT=dat; // Send the high nibble
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the high-nibble transfer

	LCD1602_DATAPORT=dat<<4; // Send the low nibble
	delay_ms(1);
	LCD1602_E=1; // Generate the enable pulse
	delay_ms(1);
	LCD1602_E=0; // Finish the low-nibble transfer
}
#endif

/*
 * Function: lcd1602_init
 * Description: Initialize the LCD1602 for two-line text display.
 */
#if (LCD1602_4OR8_DATA_INTERFACE==0) // 8-bit LCD interface
void lcd1602_init(void)
{
	lcd1602_write_cmd(0x38); // 8-bit interface, 2 lines, 5x7 font
	lcd1602_write_cmd(0x0c); // Display on, cursor off, blink off
	lcd1602_write_cmd(0x06); // Increment the cursor after each write
	lcd1602_write_cmd(0x01); // Clear the display
}
#else // 4-bit LCD interface
void lcd1602_init(void)
{
	lcd1602_write_cmd(0x28); // 4-bit interface, 2 lines, 5x7 font
	lcd1602_write_cmd(0x0c); // Display on, cursor off, blink off
	lcd1602_write_cmd(0x06); // Increment the cursor after each write
	lcd1602_write_cmd(0x01); // Clear the display
}
#endif

/*
 * Function: lcd1602_clear
 * Description: Clear the LCD1602 display.
 */
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);
}

/*
 * Function: lcd1602_show_string
 * Description: Write a null-terminated string starting at the specified
 *              row and column. Text wraps between the two LCD rows.
 * Parameters:
 *   x   - Starting column from 0 to 15.
 *   y   - Starting row: 0 for the first row, 1 for the second row.
 *   str - Pointer to the null-terminated string.
 */
void lcd1602_show_string(u8 x,u8 y,u8 *str)
{
	u8 i=0;

	if(y>1||x>15)return; // Ignore coordinates outside the display

	if(y<1) // Write starting from the first row
	{
		while(*str!='\0') // Stop at the end of the string
		{
			if(i<16-x) // Continue on the first row
			{
				lcd1602_write_cmd(0x80+i+x); // Set the first-row address
			}
			else
			{
				lcd1602_write_cmd(0x40+0x80+i+x-16); // Wrap to the second row
			}
			lcd1602_write_data(*str); // Write the current character
			str++; // Advance to the next character
			i++;
		}
	}
	else // Write starting from the second row
	{
		while(*str!='\0')
		{
			if(i<16-x) // Continue on the second row
			{
				lcd1602_write_cmd(0x80+0x40+i+x); // Set the second-row address
			}
			else
			{
				lcd1602_write_cmd(0x80+i+x-16); // Wrap to the first row
			}
			lcd1602_write_data(*str);
			str++;
			i++;
		}
	}
}
