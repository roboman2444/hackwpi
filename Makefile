CC = gcc
LDFLAGS = -lGL -lGLEW -lglfw -lm
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native
OBJECTS = hack.o tex.o stb_image_precompiled.o mathlib.o shadermanager.o framebuffers.o glhelp.o camera.o matrixlib.o

LDFREENECT = /usr/local/lib/libfreenect.so -lpthread
# Change this to freenect directory
INCLUDEFREENECT = -I/home/lambert/workspace/etc/libfreenect/include
FREENECTOBJ = \
			libfreenect_buffer.o \
			libfreenect_sync.o \

ASDFG = freenect_sync/libfreenect_sync.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

freenect:
	$(CC) $(CFLAGS) -c -o libfreenect_buffer.o freenect_sync/libfreenect_buffer.c
	$(CC) $(CFLAGS) -c -o libfreenect_sync.o freenect_sync/libfreenect_buffer.c $(INCLUDEFREENECT)	

hack: $(OBJECTS) freenect
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

solar: solar.o
	$(CC) $(CFLAGS) solar.o -o $@ $(LDFLAGS)

debug:	CFLAGS= -Wall -O0 -g  -fstrict-aliasing -march=native
debug: 	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o hack-$@ $(LDFLAGS)


clean:
	@echo cleaning oop
	@rm -f $(OBJECTS)
	@rm -f solar.o
	@rm -f $(notdir $(FREENECTSRC:.c=.o))
purge:
	@echo purging oop
	@rm -f $(OBJECTS)
	@rm -f hack
	@rm -f solar
	@rm -f hack-debug
