#FSNav Makefile 64 bit (by genBTC) 12-9-17 
# for MSYS2 MingW for Windows

csrc = $(wildcard src/*.c src/vmath/*.c src/imago/*.c)
ccsrc = $(wildcard src/*.cc src/vmath/*.cc src/imago/*.cc)
cobj = $(csrc:.c=.o)
ccobj = $(ccsrc:.cc=.o)
bin = fsnav
PREFIX = /usr/local
##DEFINES:
def = -DFREEGLUT_STATIC -DGRAPHITE2_STATIC -DPREFIX=\"$(PREFIX)\"
#there is a bug in the harfbuzz package for msys that prevents static linking and has a circular dependency of freetype->harfbuzz->freetype->graphite
#This may be fixed soon, for now just compile with dynamic and use the DLL's, or recompile harfbuzz yourself
#i forgot what this means exactly.

##INCLUDES: Set this up for your MingW dir (no ending slash)
MingDir = C:/Software/msys64
MingPrefix = /mingw64
MingArch = x86_64-w64-mingw32
ifndef MSYSTEM_PREFIX
    MSYSTEM_PREFIX = $(MingDir)$(MingPrefix)
endif
ifndef MSYSTEM_CHOST
    MSYSTEM_CHOST = $(MingArch)
endif  
prfx_inc = -I$(MSYSTEM_PREFIX)/include -I$(MSYSTEM_PREFIX)/$(MSYSTEM_CHOST)/include
incdirs = -Isrc -Isrc/vmath -Isrc/imago $(prfx_inc)

#compiler commands
CC = gcc
CXX = g++
CFLAGS = -g -O3 $(incdirs) $(def) `pkg-config --cflags freetype2`
CXXFLAGS = $(CFLAGS)

##LINKED LIBRARIES:
libdirs = -L$(MSYSTEM_PREFIX)/lib -L$(MSYSTEM_PREFIX)/$(MSYSTEM_CHOST)/lib
GL_LIBS = -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32 -Wl,--subsystem,windows
LDFLAGS = $(libdirs) $(GL_LIBS) `pkg-config --libs freetype2` -lpng16 -ljpeg -lm

$(bin): $(cobj) $(ccobj)
	$(CXX) -o $@ $(cobj) $(ccobj) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(cobj) $(ccobj) $(bin)

.PHONY: install
install:
	install -m 755 $(bin) $(PREFIX)/bin/$(bin)
	install -d $(PREFIX)/share/fsnav
	install -m 644 data/* $(PREFIX)/share/fsnav/

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/$(bin)
	rm -f $(PREFIX)/share/fsnav/*
	rmdir $(PREFIX)/share/fsnav
