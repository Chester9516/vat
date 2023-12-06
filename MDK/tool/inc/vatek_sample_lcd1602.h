#include "main.h"
#include "gpio.h"

#define LCD_RW_H HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_RW_ON_Pin,GPIO_PIN_SET)
#define LCD_RW_L HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_RW_ON_Pin,GPIO_PIN_RESET)
#define LCD_RS_DATA HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_RS_Pin,GPIO_PIN_SET)
#define LCD_RS_CMD HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_RS_Pin,GPIO_PIN_RESET)
#define LCD_ENABLE HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_EE_ON_Pin,GPIO_PIN_SET)
#define LCD_DISABLE HAL_GPIO_WritePin(LCD_GPIO_Port,LCD_EE_ON_Pin,GPIO_PIN_RESET)

char vatek_lcd_init();
void lcd_write_data(char data);
void lcd_write_cmd(char cmd);
char vatek_lcd_set_cursor(char row, char cal);
char vatek_lcd_clear();
char vatek_lcd_write_str(char *word);
