SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS   = -Wall -Wextra -no-pie -mtune=native `sdl2-config --cflags`
LDFLAGS8 = `sdl2-config --libs` -lm -ldl -lSDL2_image
srcdir	 =src/
builddir =build/



TARGETS	 =   sdl2_glad sdl2_glad1



.PHONY: all
all: $(TARGETS)

sdl2_glad: $(srcdir)glad.c $(srcdir)helper.c $(srcdir)sdl2_glad.c
	$(CC) $(CFLAGS) -o $(builddir)$@ $+ $(LDFLAGS8)

sdl2_glad1: $(srcdir)glad.c $(srcdir)helper.c $(srcdir)sdl2_glad1.c
	$(CC) $(CFLAGS) -o $(builddir)$@ $+ $(LDFLAGS8)

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true
