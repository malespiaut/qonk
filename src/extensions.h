// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <SDL/SDL.h>

double frand();
double frand( double max );
Uint32 HSVtoRGB( float h, float s, float v );

Uint8 getRed( Uint32 color );
Uint8 getGreen( Uint32 color );
Uint8 getBlue( Uint32 color );

#endif
