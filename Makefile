PREFIX=/home/rob/INSTALL/qonk
BIN=/home/rob/INSTALL/qonk/bin

CXX=g++
CFLAGS=`sdl-config --cflags` -O3
LIBS=`sdl-config --libs` -lSDL_ttf -lSDL_gfx

WCC=i586-mingw32msvc-g++
WC=i586-mingw32msvc-gcc
WCFLAGS=-c
WLDFLAGS=-lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx
WLIBS=-lmingw32 -lSDLmain -lSDL -lSDL_ttf -lSDL_gfx

OBJECTS=extensions.o coordinate.o stars.o planets.o selection.o ships.o main.o actions.o players.o universe.o animations.o fonts.o messages.o timer.o config.o ReadConf.o

all: qonk

qonk: $(OBJECTS)
	$(CXX) $(CFLAGS) $(LIBS) $(OBJECTS) -o qonk

windows: wob
	$(WCC) $(WLDFLAGS) $(OBJECTS) -o qonk.exe $(WLIBS)

wob:
	$(WCC) $(WCFLAGS) *.cpp

.cpp.o:
	$(CXX) $(CFLAGS) -c $<

clean:
	@rm -vf *.o *~ qonk screenshot.bmp DEADJOE

dist: clean
	mkdir -p qonk/
	cp *.cpp qonk/
	cp *.h qonk/
	cp Makefile qonk/
	cp TODO qonk/
	cp README qonk/
	cp COPYING qonk/
	cp font.ttf qonk/
	cp qonk.conf qonk/
	tar -cvzf ../qonk.tar.gz qonk/
	rm -rf qonk/

install: all
	mkdir -p $(PREFIX)/qonk
	mkdir -p $(BIN)
	cp qonk $(PREFIX)/qonk/
	cp font.ttf $(PREFIX)/qonk/	
	cp qonk.conf $(PREFIX)/qonk/	
	echo "cd $(PREFIX)/qonk" > $(BIN)/qonk
	echo "./qonk" >> $(BIN)/qonk
	chmod +x $(BIN)/qonk
	
	
uninstall:
	rm -rf $(PREFIX)/qonk
	rm $(BIN)/qonk
