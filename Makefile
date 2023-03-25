CC = gcc -I ./include -c -DDEBUG -fsanitize=address -g
LD = gcc -fsanitize=address -g


SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



%.o: %.c
	$(CC) $< -o $@

all: $(OBJECTS)
	$(LD) $^ -o readelf
	./readelf ../ld/func.elf


clean:
	rm *.o ld -rf
