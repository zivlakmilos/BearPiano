CC = avr-gcc
CCFLAGS = -mmcu=atmega328p -DF_CPU=16000000UL
OBJCOPY = avr-objcopy

SRC = \
	main.c \
	capacitive_touch.c

OBJS = $(SRC:.c=.o)

LIBS =

bearpiano.hex: bearpiano.elf
	$(OBJCOPY) -j .text -j .data -O ihex bearpiano.elf bearpiano.hex

bearpiano.elf: $(OBJS)
	$(CC) $(CCFLAGS) -o bearpiano.elf $^ $(LIBS)

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

flash: bearpiano.hex
	avrdude -p atmega328p -c usbasp -e -U flash:w:bearpiano.hex:i

install: flash

clean:
	rm -Rf *.hex *.elf *.o
