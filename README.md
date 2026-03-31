# Basic AVR Password Lock

First project using **ATmega328**, practicing on learning the AVR framework. As well as learning how to use EEPROM read and write function to allow the system to remember a state even after the system is turned off.

##  Features

* **EEPROM Persistence:** Stores the 4-digit password in non-volatile memory. The password remains saved even after power-off.
* **Direct Verification:** Compares keypad input directly against EEPROM values to minimize SRAM footprint.
* **Input Handling:** Supports `*` for clearing the current buffer and `#` for verifying the password.

## Tools Used
1. SimulIDE - 1.1.0-SR1
2. Visual Studio Code

## 📺 Live Demo
[Instagram Video](https://your-link-here.com)