#ifndef DISPLAY_LCD_H
#define DISPLAY_LCD_H

void display_to_LCD(char *str);
void lcd_command(uint8_t cmd);
void lcd_init();

#endif