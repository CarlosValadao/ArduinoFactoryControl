

all:
	avr-gcc -mmcu=atmega328p -Os -o main.elf main.c DHT.c atmega328p_usart.c bit_utils.c  && avr-objcopy -O ihex -R .eeprom main.elf main.hex

default:
	avr-gcc -O1 -mmcu=atmega328p -c main.c -o main.o 
	avr-gcc -O1 -mmcu=atmega328p main.o -o main.elf 
	avr-objcopy -O ihex main.elf main.hex