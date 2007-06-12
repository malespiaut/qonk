// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef STARS_H
#define STARS_H

#include <vector>

#include <SDL/SDL.h>

#include "coordinate.h"

class Star {
private:
	Uint8 brightness;
	Coordinate c;
public:
	Star();
	void render( SDL_Surface* screen ) const;
};

class Stars : public std::vector< Star > {
public:
	Stars();
	Stars( int n );
	
	void addStars( int n );
	
	void render( SDL_Surface* screen ) const;
};

#endif
