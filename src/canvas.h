// Copyright 2007 by Robert Schuster <robertschuster@fsfe.org>

// Basic drawing and converting operations for Qonk

// If not explicitly told otherwise coordinate values are
// in game units.

#ifndef CANVAS_H
#define CANVAS_H

#include <SDL/SDL.h>

class Coordinate;

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

    static SDL_Surface *initScreen();

  private:

    static SDL_Surface* main;

};

#endif /* CANVAS_H */
