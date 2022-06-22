/*
 * SSD1306Lib.c
 *
 * Created: 22.06.2022 12:39:06
 * Author : Enes
 */ 

#include "oled/oled.h"
#include "oled/seymbols.h"

char buffer[20], float_[15];


int main(void)
{
    lcd_drawBitmap(nothing_big,64,128,0,0); // clear draw
	lcd_show();
	
	
	lcd_drawBitmap(gsgk,64,128,0,0);// show seymbols
	lcd_show();
	
	lcd_clear_screen(); // clear screen
	
	lcd_drawBitmap(nothing_big,64,128,0,0); // clear draw
    lcd_show();
	
	
	lcd_gotoxy(0,7);
	lcd_puts("example"); // show text
	
	lcd_write_delay(2); // show big number
	
	
	lcd_gotoxy(0,7);
	sprintf(buffer,"weather %d°",20); // show with sprintf
	lcd_puts(buffer);
	
    while (1) 
    {
    }
}

