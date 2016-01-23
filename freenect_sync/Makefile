CC = gcc
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native

OBJECTS = libfreenect_buffer.o libfreenect_sync.o

all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o ../$@ $<
