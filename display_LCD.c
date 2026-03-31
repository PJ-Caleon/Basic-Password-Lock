#include <avr/io.h>
#include <util/delay.h>

#include "display_LCD.h"

#define LCD_PORT PORTC
#define LCD_DDR DDRC
#define RS PC0
#define EN PC5

void lcd_pulse()
{
    LCD_PORT |= (1 << EN);
    _delay_ms(1);
    LCD_PORT &= ~(1 << EN);
    _delay_ms(1);
}

void lcd_send_nibble(uint8_t nibble) {
    LCD_PORT &= ~0x1E; 
    
    if (nibble & 0x01) LCD_PORT |= (1 << PC4);
    if (nibble & 0x02) LCD_PORT |= (1 << PC3);
    if (nibble & 0x04) LCD_PORT |= (1 << PC2);
    if (nibble & 0x08) LCD_PORT |= (1 << PC1);
    
    lcd_pulse();
}

void lcd_command(uint8_t cmd)
{
    LCD_PORT &= ~(1 << RS);
    lcd_send_nibble(cmd >> 4);
    lcd_send_nibble(cmd);
    _delay_ms(2);
}

void lcd_data(uint8_t data)
{
    LCD_PORT |= (1 << RS);
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data);
    _delay_ms(2);
}

void lcd_init() {
    LCD_DDR = 0x3F; 
    _delay_ms(100);

    lcd_send_nibble(0x03); 
    _delay_ms(10);
    
    lcd_send_nibble(0x02); 
    _delay_ms(5);

    lcd_command(0x28); 
    lcd_command(0x0C); // Display ON, Cursor OFF
    lcd_command(0x06); // Entry Mode
    lcd_command(0x01); // Clear
    _delay_ms(2);
}

void display_to_LCD(char *str)
{
    while (*str)
    {
        lcd_data(*str++);
    }
}