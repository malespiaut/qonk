// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef STARS_H
#define STARS_H

#include <vector>

#include "coordinate.h"

class Star {
private:
	int brightness;
	Coordinate c;
public:
	Star();
	void render( ) const;
};

class Stars : public std::vector< Star > {
public:
	Stars();
	Stars( int n );
	
	void addStars( int n );
	
	void render( ) const;
};

#endif
