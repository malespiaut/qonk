// Copyright 2007 by Robert Schuster <robertschuster@fsfe.org>

// Basic drawing and converting operations for Qonk

// If not explicitly told otherwise coordinate values are
// in game units.

#ifndef CANVAS_H
#define CANVAS_H

#include <SDL/SDL.h>

#include "guichan/guichan/sdl.hpp"

class Coordinate;
class Font;

class Canvas
{
  public:
    static void alphaBlend( SDL_Surface* surface, Uint8 alpha );

    static void drawSelector( Coordinate&, Sint16 offset,
                              Sint16 width, Sint16 height,
			      Uint8 R, Uint8 G, Uint8 B );

    static void drawNearestPlanetSelector(Coordinate&, int);

    static void drawPlanet(Coordinate&, int size, Uint32 color);

    static void drawPlanetMapped(int x, int y, int size, Uint32 color);

    static void drawResidentShip(Coordinate& shipLocation, Coordinate& planetLocation,
				 int color);

    static void drawFlyingShip(Coordinate&, double direction, int color);

    static void drawSelection(Coordinate&);

    static void drawOrbit(Coordinate&, double rotationDistance, int color);

    static void drawBuildProgress(Coordinate&, int, double);

    static void drawText(int, int, const char *, int, int, int, int a=255);

    static int getFontHeight();

    static void drawBox(int x, int y, int w, int h, int r, int g, int b);

    static void drawRadar();

    static void drawSun();

    static void drawStar(int x, int y, int brightness);
 
    static void drawSonar(Coordinate coord, int size, double percentage, int r, int g, int b, bool circle);

    static void drawPlayerStat(int size, int index, int previousValue, int currentValue, int r, int g, int b);

    static void drawMouseSelection(Coordinate &c1, Coordinate &c2);

    static void drawCursor(int, int);

    static void updateScreen();

    static void initScreen();

    static void shutdown();

    static gcn::SDLGraphics *getSDLGraphics();

  private:

    static gcn::SDLGraphics *sdlGraphics;

    static SDL_Surface* main;

    static Font *font;

};

#endif /* CANVAS_H */
