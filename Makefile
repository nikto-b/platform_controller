MAINFILENAME=main
MCU=atmega328p
OPTIMIZE=-Os
CFLAGS=-c $(OPTIMIZE) -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -fno-devirtualize -fno-use-cxa-atexit -mmcu=$(MCU) -DF_CPU=$(XTAL)
LFLAGS=$(OPTIMIZE) -Wall -Wextra -flto -fuse-linker-plugin -ffunction-sections -fdata-sections -Wl,--gc-sections -mmcu=$(MCU) -lm

all: size

main: avr-api
	avr-g++ $(CFLAGS) "$(MAINFILENAME).cpp" -o "$(MAINFILENAME).o"

arch: main avr-api

link: arch
	avr-g++ -std=gnu++11 -Wall -Wextra $(OPTIMIZE) -g -flto -fuse-linker-plugin -ffunction-sections -fdata-sections -Wl,--gc-sections -mmcu=$(MCU) main.o ./avr-api/core.a -o main.elf -L./avr-api -lm

objcopy: link
	avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  "$(MAINFILENAME).elf" "$(MAINFILENAME).eep"
	avr-objcopy -O ihex -R .eeprom  "$(MAINFILENAME).elf" "$(MAINFILENAME).hex"


size: objcopy
	avr-size -C $(MAINFILENAME).elf --mcu=$(MCU)


.PHONY avr-api:
	make -C ./avr-api clean
	make -C ./avr-api lib

clean:
	make -C ./avr-api clean
	rm -f main.o
