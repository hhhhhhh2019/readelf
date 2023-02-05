CC = gcc -I ./include -c -DDEBUG -fsanitize=address -g
LD = gcc -lasan -g


SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)



%.o: %.c
	$(CC) $< -o $@

all: $(OBJECTS)
	$(LD) $^ -o readelf


clean:
	rm *.o ld -rf
