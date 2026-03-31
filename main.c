#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include "input_LCD.h"
#include "display_LCD.h"
#include "memory.h" 

int main(void)
{
    lcd_init();
    char input_buffer[5] = {'\0'};
    uint8_t count = 0;

    // Setup PB7 for "Change Password" mode
    DDRB &= ~(1 << PB7);
    PORTB |= (1 << PB7); 

    // Setup PB4 and PB5 for LED output
    DDRB |= (1 << PB4) | (1 << PB5);
    PORTB &= ~((1 << PB4) | (1 << PB5));

    // Setup PB3 for DC motor
    DDRB |= (1 << PB3);
    PORTB &= ~(1 << PB3);

    lcd_command(0x01);
    display_to_LCD("Loading");
    
    for(uint8_t i = 0; i < 5; i++) {
        _delay_ms(400); 
        display_to_LCD(".");
    }
    _delay_ms(500);
    lcd_command(0x01);

    display_to_LCD("Enter Password:");
    _delay_ms(800);
    lcd_command(0x01);

    while(1)
    {
        char key = keyboard_to_LCD();
        
        if (key != 0)
        {
            if (key == '*') 
            { 
                lcd_command(0x01);
                count = 0;
                for(int i = 0; i < 5; i++) input_buffer[i] = '\0';
                display_to_LCD("Cleared");
                _delay_ms(500);
                lcd_command(0x01);
            } 
            else if (key == '#')
            {
                // Verify directly against EEPROM to save RAM
                bool match = true;
                if (count != 4) {
                    match = false;
                } else {
                    for(int i = 0; i < 4; i++) {
                        if(input_buffer[i] != read_from_eeprom(i)) {
                            match = false;
                            break;
                        }
                    }
                }

                lcd_command(0x01);
                if(match) 
                {
                    display_to_LCD("Access Granted");
                    PORTB |= (1 << PB4); 
                    PORTB &= ~(1 << PB5); 
                    PORTB |= (1 << PB3);
                    _delay_ms(3000);
                    PORTB &= ~(1 << PB4); 
                    PORTB &= ~(1 << PB3);
                }
                else
                {
                    display_to_LCD("Wrong Password");
                    PORTB |= (1 << PB5); 
                    PORTB &= ~(1 << PB4); 
                    _delay_ms(1500);
                    PORTB &= ~(1 << PB5);
                }

                _delay_ms(1000);
                lcd_command(0x01);
                
                // Reset for next attempt
                count = 0;
                for(int i = 0; i < 5; i++) input_buffer[i] = '\0';
            }
            else if (count < 4)
            {
                input_buffer[count++] = key;
                input_buffer[count] = '\0';
                lcd_command(0x01);
                display_to_LCD(input_buffer);
            }
            while(keyboard_to_LCD() != 0); 
            _delay_ms(100); 
        }

        // --- CHANGE PASSWORD MODE  ---
        if (!(PINB & (1 << PB7)))
        {
            lcd_command(0x01);
            display_to_LCD("New Password:");
            _delay_ms(1000);

            count = 0; 
            for(int i = 0; i < 5; i++) input_buffer[i] = '\0';

            while(count < 4) {
                char n_key = keyboard_to_LCD();
                if(n_key != 0 && n_key != '*' && n_key != '#') {
                    input_buffer[count++] = n_key;
                    lcd_command(0x01);
                    display_to_LCD(input_buffer);
                    while(keyboard_to_LCD() != 0);
                }
            }

            for(uint16_t i = 0; i < 4; i++) write_to_eeprom(i, input_buffer[i]);
            
            lcd_command(0x01);
            display_to_LCD("Saved!");
            _delay_ms(1500);
            
            count = 0;
            for(int i = 0; i < 5; i++) input_buffer[i] = '\0';
            lcd_command(0x01);
            while (!(PINB & (1 << PB7))); 
        }
    }
}