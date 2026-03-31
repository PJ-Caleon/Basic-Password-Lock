# Project Settings
MCU = atmega328p
F_CPU = 16000000UL
TARGET = main
SRCS = main.c input_LCD.c display_LCD.c memory.c

# Compiler Settings
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall

# --- THE CHANGE: Add .eep to the 'all' target ---
all: $(TARGET).hex $(TARGET).eep

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

# --- NEW RULE: Extract the EEPROM data ---
$(TARGET).eep: $(TARGET).elf
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O ihex $(TARGET).elf $(TARGET).eep

clean:
	rm -f *.elf *.hex *.eep *.o