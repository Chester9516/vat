#if defined(STM32F401xC)

#include "vatek_sample_lcd1602.h"

static void lcd_write(char data, int rs)
{
	HAL_GPIO_WritePin(LCD_GPIO_Port, LCD_RS_Pin, rs);
	
	HAL_GPIO_WritePin(LCD_GPIO_Port, LCD_D7_Pin, (data>>3)&0x1);
	HAL_GPIO_WritePin(LCD_GPIO_Port, LCD_D6_Pin, (data>>2)&0x1);
	HAL_GPIO_WritePin(LCD_GPIO_Port, LCD_D5_Pin, (data>>1)&0x1);
	HAL_GPIO_WritePin(LCD_GPIO_Port, LCD_D4_Pin, (data)&0x1);
	
	LCD_ENABLE;
	HAL_Delay(20);
	LCD_DISABLE;
	HAL_Delay(20);
}

void lcd_write_cmd(char cmd)
{
	char cmd_addr;
	cmd_addr = (cmd>>4)&0xf;
	lcd_write(cmd_addr, 1);
	
	cmd_addr = cmd&0xf;
	lcd_write(cmd_addr, 1);
}

void lcd_write_data(char data)
{
	char data_val;
	data_val = (data>>4)&0x0f;
	lcd_write(data_val, 0);
	
	data_val = data&0x0f;
	lcd_write(data_val, 0);
}

char vatek_lcd_init()
{
	lcd_write_cmd(0x30);
	HAL_Delay(50);
	lcd_write_cmd(0x30);
	HAL_Delay(50);
	lcd_write_cmd(0x30);
	HAL_Delay(50);
	lcd_write_cmd(0x20);
	HAL_Delay(50);
	
	lcd_write_cmd(0x28);
	HAL_Delay(1);
	lcd_write_cmd(0x08);
	HAL_Delay(1);
	lcd_write_cmd(0x01);
	HAL_Delay(1);
	lcd_write_cmd(0x06);
	HAL_Delay(1);
	lcd_write_cmd(0x0c);
	HAL_Delay(1);
	
	return 1;
}

char vatek_lcd_clear()
{
	lcd_write_cmd(0x1);
	HAL_Delay(50);
}

char vatek_lcd_set_cursor(char row, char cal)
{
	char position;
	
	if(row == 0){
		position = 0x00 | cal;
	}
	else
		position = 0x40 | cal;
	
	lcd_write_cmd(position | 0x80);
}

char vatek_lcd_write_str(char *word)
{
	while(*word)
		lcd_write_data(*word++);
}

char vatek_set_blank(char row, char cal, uint32_t blank_num)
{
//	vatek_lcd_set_cursor(row, cal);
//	vatek_lcd_write();
}

#endif