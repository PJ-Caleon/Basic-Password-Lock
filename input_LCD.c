#include <avr/io.h>
#include <util/delay.h>

#include "input_LCD.h"

#define KEYPAD_PORT PORTD
#define KEYPAD_PIN PIND
#define KEYPAD_DDR DDRD

char keys[4][3] = 
{
    {'3', '2', '1'},
    {'6', '5', '4'},
    {'9', '8', '7'},
    {'#', '0', '*'}
};

unsigned char keypad_scan()
{
    uint8_t old_UCSR0B = UCSR0B;
    UCSR0B &= ~(1 << TXEN0);

    for (uint8_t row = 0; row < 4; row++) 
    {
        DDRD |= 0x0F;   // Pins 0-3 as Output
        PORTD = 0xFF;   // All Pull-ups/High
        PORTD &= ~(1 << row); // Pull current row LOW
        
        _delay_us(5); 

        for (uint8_t col = 0; col < 3; col++) 
        {
            uint8_t current_col_pin = 4 + col; 
            
            if (!(PIND & (1 << current_col_pin))) 
            {
                UCSR0B = old_UCSR0B;
                return keys[row][col]; 
            }
        }
    }
    UCSR0B = old_UCSR0B;
    return 0;
}

char keyboard_to_LCD()
{
    return keypad_scan();
}