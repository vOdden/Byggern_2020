
#include <avr/pgmspace.h>
#include <stdarg.h>
#include <stdio.h>

#include "OLED.h"
#include "fonts/fontdescr.h"
#include "string.h"
#include "fonts.h"

// static uint8_t column   = 0;
// static FILE oled_stdout = FDEV_SETUP_STREAM(OLED_write_char, NULL, _FDEV_SETUP_WRITE);

static FontDescr font;

///////////////////////////////////////////////////////

void OLED_init()
{
	OLED_write_cmd(0xae); //	display off
	OLED_write_cmd(0xa1); //	segment remap
	OLED_write_cmd(0xda); //	common pads hardware: alternative
	OLED_write_cmd(0x12);
	OLED_write_cmd(0xc8); //	common output scan direction:com63-com()
	OLED_write_cmd(0xa8); //	multiplex ration mode:63
	OLED_write_cmd(0x3f); //
	OLED_write_cmd(0xd5); //	display divide ratio/osc. freq. mode
	OLED_write_cmd(0x80);
	OLED_write_cmd(0x81); //	contrast control
	OLED_write_cmd(0x50);
	OLED_write_cmd(0xd9); //	set pre-charge period
	OLED_write_cmd(0x21);
	OLED_write_cmd(0x20); //	Set Memory Addressing Mode
	OLED_write_cmd(0x02);
	OLED_write_cmd(0xdb); //	VCOM deselect level mode
	OLED_write_cmd(0x30);
	OLED_write_cmd(0xad); //	master configuration
	OLED_write_cmd(0x00);
	OLED_write_cmd(0xa4); //	out follows RAM content
	OLED_write_cmd(0xa6); //	set normal display
	OLED_write_cmd(0xaf); //	display on
	
	OLED_clear();
	OLED_home();
};


void OLED_write_cmd(char cmd)
{
	*OLED_CMD_ADDRESS = cmd;
}


void OLED_clear(void){
    
	for(uint8_t i = 0; i < OLED_LINES; i++)
	{
        OLED_clear_line(i);
    }
	OLED_home();
}

void OLED_clear_line(uint8_t line)
{
    OLED_goto(line, 0);
    
	for(int col = 0; col < OLED_WIDTH; col++)
	{
        OLED_WRITE_ADDRESS[col] = 0x00;
    }
	OLED_goto(line, 0);
}

void OLED_home(void)
{
	OLED_goto(0,0);
}

void OLED_goto(uint8_t line, uint8_t column)
{
	OLED_goto_line(line);
	OLED_goto_column(column);
}

void OLED_goto_line(uint8_t line)
{
	if(line < OLED_LINES)
	{
		OLED_write_cmd(0xB0 + line);	// Page start address
		//current_line = line;
	}
}

void OLED_goto_column(uint8_t column)
{
	if(column < OLED_WIDTH)
	{
		OLED_write_cmd(0x00 + (column % 16));			// Set lower column start address
		OLED_write_cmd(0x10 + (column / 16));			// Set higher column start address
	}
}

void OLED_font_select(FontDescr fd)
{
    font = fd;
}

void OLED_write_char(char c)
{
	{
        for (uint8_t i = 0; i < 8; i++)
		{
 //           OLED_WRITE_ADDRESS[0] = (pgm_read_byte( font.addr + (c-font.start_offset)*font.width + i ));
			OLED_WRITE_ADDRESS[i] = pgm_read_byte(&font8[c - 32][i]); //fontx er peker til aktuell font
        }
    }
}





void OLED_INV_write_char(char c)
{
        for (uint8_t i = 0; i < 8; i++)
		{
			OLED_WRITE_ADDRESS[i] = ~pgm_read_byte(&font8[c - 32][i]); //fontx er peker til aktuell font
        }
}



void OLED_write_string(char *s)
{
	for(int col = sizeof(s); col < 127; col ++)
	{
		*OLED_WRITE_ADDRESS = 0x00;
	}
	OLED_goto_column(0);
	while(*s)
	{
		OLED_write_char(*s++);
	}

}

void OLED_INV_write_string(char *s)
{
	for(int col = sizeof(s); col < 127; col ++)
	{
		*OLED_WRITE_ADDRESS = 0xFF;
	}
	OLED_goto_column(0);
	while(*s)
	{
		OLED_INV_write_char(*s++);
	}
}

void OLED_Allign_centre(char* title)
{
		OLED_goto_column(64 - 8*strlen(title)/2); //5 = fontsize
}


int long_string(char* data)
{
	if(strlen(data)*8 > 128)// 8 = fontsize
	{ 
		return 1;
	}
	return 0;
}

void OLED_fill_line(int line){
	OLED_goto(line, 0);
	
	for(int col = 0; col < 128; col ++)
	{
		*OLED_WRITE_ADDRESS = 0xFF;
	}
	OLED_goto_line(line);
}