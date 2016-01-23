CC = gcc
LDFLAGS = -lGL -lGLEW -lglfw -lm
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native
OBJECTS = hack.o tex.o stb_image_precompiled.o mathlib.o shadermanager.o framebuffers.o glhelp.o camera.o matrixlib.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

hack: $(OBJECTS)
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
purge:
	@echo purging oop
	@rm -f $(OBJECTS)
	@rm -f hack
	@rm -f solar
	@rm -f hack-debug
