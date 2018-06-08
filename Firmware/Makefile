PROJECT = main

MCU = atmega8
CLOCK = 11059200UL
LFUSE = 0xff
HFUSE = 0xd9

PROGRAMMER = avrdude
ISP = usbasp
SCK = 0.5

CC = avr-g++
OBJCOPY = avr-objcopy
CFLAGS = -Os -mcall-prologues -std=c++11 -mmcu=$(MCU) -DF_CPU=$(CLOCK) -I. $(DEBUG)
DEBUG = #-DDEBUG# -Wl,-u,vfprintf -lprintf_flt -lm
MAP = #-Wl,-Map,$(PROJECT).map

OBJECTS = \
		lib/HIIC.o\
		lib/HUART.o\
		device/DS1307.o\
		device/LED.o\
		application/RTC.o\
		application/Terminal.o\
		application/App.o\
		main.o

all: $(PROJECT).hex

$(PROJECT).hex : $(PROJECT).bin
	$(OBJCOPY)  -j .text -j .data -O ihex $(PROJECT).bin $(PROJECT).hex

$(PROJECT).bin : $(OBJECTS)
	$(CC) $(CFLAGS) $(MAP) -o $(PROJECT).bin $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.S.o:
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(CC) $(CFLAGS) -S $< -o $@

flash: $(PROJECT).hex
	$(PROGRAMMER) -c $(ISP) -p $(MCU) -e -U flash:w:$(PROJECT).hex -B $(SCK)

fuse:
	$(PROGRAMMER) -c $(ISP) -p $(MCU) -e -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -B 32

lock:
	$(PROGRAMMER) -c $(ISP) -p $(MCU) -U lock:w:0x00:m -B $(SCK)

clean:
	rm -f *.o */*.o *.map *.bin *.hex
