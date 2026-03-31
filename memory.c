#include <avr/io.h>

void write_to_eeprom(uint16_t address, uint8_t data)
{
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Setup address and data registers
    EEAR = address;
    EEDR = data;

    // Enable master write and start write
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

uint8_t read_from_eeprom(uint16_t address)
{
    // Wait for completion of previous write
    while(EECR & (1 << EEPE)); 

    EEAR = address; // Set address
    EECR |= (1 << EERE); // Start read

    return EEDR; // Return data
}