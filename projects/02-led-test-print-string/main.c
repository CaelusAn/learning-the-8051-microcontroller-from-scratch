#include "lcd1602.h"   //  public.h -> reg52.h

void main()
{
    lcd1602_init();
    lcd1602_show_string(0, 0, "114514");
    lcd1602_show_string(0, 1, "1919810");
    while(1)
    {
			
    }
}