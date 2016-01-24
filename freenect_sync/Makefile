CC = gcc
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native

OBJECTS = libfreenect_buffer.o

all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o ../$@ $< -lpthread -lfreenect
