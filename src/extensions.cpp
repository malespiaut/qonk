// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include <SDL/SDL_gfxPrimitives.h>

#include "extensions.h"

double
frand() {
	return (double)rand() / RAND_MAX;
}

double
frand( double max ) {
	return max * frand();
}

Uint32 HSVtoRGB( float h, float s, float v )
{
	int i;
	float f, p, q, t, r, g, b;

	if( s == 0 ) {

		// achromatic (grey)
		r = g = b = v;

	} else {

		h /= 60;			// sector 0 to 5
		i = (int)h;
		f = h - i;			// factorial part of h
		p = v * ( 1 - s );
		q = v * ( 1 - s * f );
		t = v * ( 1 - s * ( 1 - f ) );
	
		switch( i ) {
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			default:		// case 5:
				r = v;
				g = p;
				b = q;
			break;
		}

	}

	return 256 * 256 * (int)( 255 * r ) + 256 * (int)( 255 * g ) + (int)( 255 * b );

}

Uint8 getRed( Uint32 color ) {
	return ( color & 0xFF0000 ) >> 16;
}

Uint8 getGreen( Uint32 color ) {
	return ( color & 0x00FF00 ) >> 8;
}

Uint8 getBlue( Uint32 color ) {
	return color & 0x0000FF;
}

