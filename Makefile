CC = gcc
# Replace /usr/local/lib/libfreenect.so with -lfreenect when not using fakenect
# LDFLAGS = -lGL -lGLEW -lglfw -lm -lfreenect -lpthread
# LDFLAGS = -lGL -lGLEW -lglfw -lm /usr/local/lib/libfreenect.so -lpthread
LDFLAGS = -lGL -lGLEW -lglfw -lm -lfreenect -lpthread
CFLAGS = -Wall -Ofast -fstrict-aliasing -march=native
OBJECTS = hack.o tex.o stb_image_precompiled.o mathlib.o shadermanager.o framebuffers.o glhelp.o camera.o matrixlib.o depthback.o postprocess.o glstates.o fluids/fluids.o
OBJECTS_FREENECT = libfreenect_buffer.o 

FREENECT_DIR = freenect_sync

%.o: %.c
	$(CC) $(CFLAGS) -c -o $(notdir $@) $<

hack: $(OBJECTS) freenect
	$(CC) $(CFLAGS) $(notdir $(OBJECTS)) $(OBJECTS_FREENECT) -o $@ $(LDFLAGS)

freenect:
	make -C $(FREENECT_DIR)

solar: solar.o
	$(CC) $(CFLAGS) solar.o -o $@ $(LDFLAGS)

debug:	CFLAGS= -Wall -O0 -g  -fstrict-aliasing -march=native
debug: 	$(OBJECTS)
	$(CC) $(CFLAGS) $(notdir $(OBJECTS)) -o hack-$@ $(LDFLAGS)


clean:
	@echo cleaning oop
	@rm -f $(OBJECTS)
	@rm -f solar.o
	@rm -f $(OBJECTS_FREENECT)
purge:
	@echo purging oop
	@rm -f $(OBJECTS)
	@rm -f hack
	@rm -f solar
	@rm -f hack-debug
