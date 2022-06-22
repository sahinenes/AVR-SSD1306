/*
 * oled.cpp
 *
 * Created: 28.03.2022 15:07:46
 *  Author: Enes
 */ 

#include "oled/oled.h"
#include "i2c.h"
#include "oled/font32.h"
#include "oled/ascii_6x8.h"

 uint8_t buffer[1024];
 
const char char_width[] = {13,5,10,11,13,11,12,11,12,12,4};
const char* char_addr[] = {bitmap_48n,bitmap_49n,bitmap_50n,bitmap_51n,bitmap_52n,bitmap_53n,bitmap_54n,bitmap_55n,bitmap_56n,bitmap_57n,bitmap_58n};
	
	
#define CHAR_HEIGHT CHAR_HEIGHT_32
#define CHAR_WIDTH CHAR_WIDTH_32

static uint8_t displayBuffer[DISPLAY_HEIGHT/8][DISPLAY_WIDTH];

void lcd_init(uint8_t dispAttr){
     I2C_Init();
    uint8_t commandSequence[sizeof(init_sequence)+1];
    for (uint8_t i = 0; i < sizeof (init_sequence); i++) {
        commandSequence[i] = (pgm_read_byte(&init_sequence[i]));
    }
    commandSequence[sizeof(init_sequence)]=(dispAttr);
    lcd_command(commandSequence, sizeof(commandSequence));
    lcd_clrscr();
}

void lcd_home(void){
	lcd_gotoxy(0, 0);
}

void lcd_command(uint8_t cmd[], uint8_t size) {
    I2C_Start((LCD_I2C_ADR << 1) | 0);
    I2C_Write(0x00);	// 0x00 for command, 0x40 for data
    for (uint8_t i=0; i<size; i++) {
        I2C_Write(cmd[i]);
    }
    I2C_Stop();
}
void lcd_data(uint8_t data[], uint16_t size) {
    I2C_Start((LCD_I2C_ADR << 1) | 0);
     I2C_Write(0x40);	// 0x00 for command, 0x40 for data
    for (uint8_t i; i<size; i++) {
         I2C_Write(data[i]);
    }
    I2C_Stop();
}
void lcd_gotoxy(uint8_t x, uint8_t y){
    if( x > (DISPLAY_WIDTH) || y > (DISPLAY_HEIGHT/8-1)) return;// out of display
    //x = x * 8;					// one char: 6 pixel width
#if defined SSD1306
    uint8_t commandSequence[] = {0xb0+y, 0x21, x, 0x7f};
#elif defined SH1106
    uint8_t commandSequence[] = {0xb0+y, 0x21, 0x00+((2+x) & (0x0f)), 0x10+( ((2+x) & (0xf0)) >> 4 ), 0x7f};
#endif
    lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_clrscr(void){
    lcd_home();
    for (uint16_t i = 0; i < DISPLAYSIZE; i++) {
        lcd_data(0x01, 1);
    }

	lcd_home();
}

void lcd_putc(char c){
	if((c > 127 || 
	   c < 32) &&
	   (c != 'ü' &&
		c != 'ö' &&
		c != '°' &&
		c != 'ä' &&
		c != 'ß' &&
		c != 'Ü' &&
		c != 'Ö' &&
		c != 'Ä' ) ) return;
    
    I2C_Start((LCD_I2C_ADR << 1) | 0);
     I2C_Write(0x40);	// 0x00 for command, 0x40 for data
    	switch (c) {
		case 'ü':
			c = 95; // ü
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'ö':
			c = 99; // ö
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case '°':
			c = 101; // °
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'ä':
			c = 97; // ä
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'ß':
			c = 102; // ß
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'Ü':
			c = 96; // Ü
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'Ö':
			c = 100; // Ö
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		case 'Ä':
			c = 98; // Ä
			for (uint8_t i = 0; i < 6; i++)
			{
				I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
			}
			break;
		default:
                c -= 32;
                if( c < 127-32 ) {
                    for (uint8_t i = 0; i < 6; i++)
                    {
                        I2C_Write(pgm_read_byte(&ssd1306oled_font6x8[c * 6 + i]));	// print font to ram, print 6 columns
                    }
                    //return;
                }
			break;
	}
    I2C_Stop();
}

void lcd_puts(const char* s){
    while (*s) {
        lcd_putc(*s++);
    }
}
void lcd_puts_p(const char* progmem_s){
    register uint8_t c;
    while ((c = pgm_read_byte(progmem_s++))) {
        lcd_putc(c);
    }
}

void lcd_invert(uint8_t invert){
    I2C_Start((LCD_I2C_ADR << 1) | 0);
    uint8_t commandSequence[1];
    if (invert == YES) {
        commandSequence[0] = 0xA7;
    } else {
        commandSequence[0] = 0xA7;
    }
    lcd_command(commandSequence, 1);
}

void lcd_set_contrast(uint8_t contrast){
    uint8_t commandSequence[2] = {0x81, contrast};
    lcd_command(commandSequence, sizeof(commandSequence));
}

void lcd_put_bigc(const char* addr, uint8_t column, uint8_t char_width, uint8_t general_width, uint8_t char_height){
	uint8_t x_offset = ((general_width - char_width)>>1); 

	for (uint8_t i = 0; i < (char_height >> 3); i++) {
		lcd_gotoxy(column, 1+i); 
		I2C_Start((LCD_I2C_ADR << 1) | 0);
    	 I2C_Write(0x40);
	
		for (uint8_t j = 0; j < general_width; j++)
			if(j < x_offset) {
				I2C_Write(0x00);
				}
			else {
				if(j < (x_offset + char_width)) {
		   			I2C_Write(pgm_read_byte(&addr[j-x_offset+char_width*i])); 
					}
				else {
					I2C_Write(0x00);
					}
				}		

		I2C_Stop();		
    }
}

void lcd_clear_screen(){

	for(uint8_t i = 0; i<8; i++) {
		lcd_gotoxy(0, i);
		lcd_puts("                      ");
	}

}

void lcd_clear_bigc(uint8_t column, uint8_t char_height){
	for (uint8_t i = 0; i < (char_height >> 3) ; i++) {
		lcd_gotoxy(column, 1+i); 
		I2C_Start((LCD_I2C_ADR << 1) | 0);
    	 I2C_Write(0x40);
		for (uint8_t j = 0; j < 18; j++)
			{
   			I2C_Write(0x00); 
			}
		I2C_Stop();
		
    }
}


void lcd_update_6(uint8_t c5,uint8_t c4,uint8_t c3, uint8_t c2, uint8_t c1, uint8_t c0){
	if(c3 == 0)
	lcd_clear_bigc(0, CHAR_HEIGHT);
	else
	lcd_put_bigc (char_addr[c5], 0, char_width[c5], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c4], 18, char_width[c4], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[10], 42, char_width[10], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c3], 42, char_width[c3], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c2], 60, char_width[c2], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[10], 78, char_width[10], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c1], 85, char_width[c1], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c0], 103, char_width[c0], CHAR_WIDTH, CHAR_HEIGHT);

}



void lcd_update_4(uint8_t c3, uint8_t c2, uint8_t c1, uint8_t c0){


	lcd_put_bigc(char_addr[10], 48, char_width[10], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c3], 0, char_width[c3], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c2], 24, char_width[c2], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c1], 72, char_width[c1], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c0], 96, char_width[c0], CHAR_WIDTH, CHAR_HEIGHT);

}



void lcd_write_speed( uint8_t c0, uint8_t c1){

	lcd_put_bigc (char_addr[c0], 30, char_width[c0], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_put_bigc (char_addr[c1], 50, char_width[c1], CHAR_WIDTH, CHAR_HEIGHT);
	lcd_gotoxy(0,72);
	lcd_puts("km/h");
	

}


void lcd_write_delay( uint8_t c0){

	lcd_put_bigc (char_addr[c0], 50, char_width[c0], CHAR_WIDTH, CHAR_HEIGHT);

}


//draw image



void lcd_drawPixel(uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status) {
	if (pos_x >= DISPLAY_WIDTH || pos_y >= DISPLAY_HEIGHT) {
		return;
	}

	if (pixel_status) {
		buffer[pos_x+(pos_y/8)*DISPLAY_WIDTH] |= (1 << (pos_y&7));
		} else {
		buffer[pos_x+(pos_y/8)*DISPLAY_WIDTH] &= ~(1 << (pos_y&7));
	}
}

void lcd_show()
{
	lcd_command(0x7F,1);
	lcd_command(0x07,1);
	for (uint8_t packet = 0; packet < 64; packet++) {
		
		I2C_Start(LCD_I2C_ADR<<1 | 0);
		I2C_Write(0x40);
		for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte) {
			I2C_Write(buffer[packet*16+packet_byte]);
		}
		I2C_Stop();
	}
	
}

void lcd_drawBitmap(const uint8_t *progmem_bitmap, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y) {
	uint8_t current_byte;
	uint8_t byte_width = (width + 7)/8;

	for (uint8_t current_y = 0; current_y < height; current_y++) {
		for (uint8_t current_x = 0; current_x < width; current_x++) {
			current_byte = pgm_read_byte(progmem_bitmap + current_y*byte_width + current_x/8);
			if (current_byte & (128 >> (current_x&7))) {
				lcd_drawPixel(current_x+pos_x,current_y+pos_y,1);
				} else {
				lcd_drawPixel(current_x+pos_x,current_y+pos_y,0);
			}
		}
	}
}