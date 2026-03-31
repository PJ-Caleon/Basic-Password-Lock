#ifndef memory_h
#define memory_h

void write_to_eeprom(uint16_t address, uint8_t data);
uint8_t read_from_eeprom(uint16_t address);

#endif