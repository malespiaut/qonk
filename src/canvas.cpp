// Copyright 2005 by Anthony Liekens <anthony@liekens.net>
// Copyright 2007 by Robert Schuster <robertschuster@fsfe.org>

#include <SDL/SDL_gfxPrimitives.h>

#include "canvas.h"

#include "extensions.h"
#include "coordinate.h"
#include "settings.h"

SDL_Surface* Canvas::main = 0;

void 
Canvas::alphaBlend( SDL_Surface* surface, Uint8 alpha ) {
	if( alpha == 0 ) {
		for( int x = 0; x < surface->w; x++ )
		for( int y = 0; y < surface->h; y++ ) {
			Uint32 *bufp;
			bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
			if( *bufp != 0 )
				*bufp &= 0x00ffffff;
		}
	} if( alpha != 255 ) {
		for( int x = 0; x < surface->w; x++ )
		for( int y = 0; y < surface->h; y++ ) {
			Uint32 *bufp;
			bufp = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
			if( *bufp != 0 ) {
				// seperate RGB from A, update A for new value, OR them together again
				Uint32 color = ( *bufp & 0x00ffffff ) | ( ( *bufp >> 24 ) * alpha >> 8 ) << 24;
				*bufp = color;
			}
		}
	}
}

void
Canvas::drawSelector(Coordinate &c, Sint16 offset,
                     Sint16 width, Sint16 height, Uint8 R, Uint8 G, Uint8 B)
{
	int x1 = c.getXMapped() + offset;
	int y1 = c.getYMapped() + offset;

	hlineRGBA( main, x1, x1 + 2, y1, R, G, B, 255 );
	hlineRGBA( main, x1 + width - 2, x1 + width, y1, R, G, B, 255 );
	hlineRGBA( main, x1, x1 + 2, y1 + height, R, G, B, 255 );
	hlineRGBA( main, x1 + width - 2, x1 + width, y1 + height, R, G, B, 255 );
	vlineRGBA( main, x1, y1, y1 + 2, R, G, B, 255 );
	vlineRGBA( main, x1, y1 + height, y1 + height - 2, R, G, B, 255 );
	vlineRGBA( main, x1 + width, y1, y1 + 2, R, G, B, 255 );
	vlineRGBA( main, x1 + width, y1 + height, y1 + height - 2, R, G, B, 255 );
}

void
Canvas::drawNearestPlanetSelector(Coordinate &c, int size)
{
	size *= 3;
	int offset = -size/2;

	drawSelector(c, offset, size, size, 255, 0, 0);
}

void
Canvas::drawPlanet( Coordinate &loc, int size, Uint32 color ) {
	drawPlanetMapped(loc.getXMapped(), loc.getYMapped(), size, color);
}

void
Canvas::drawPlanetMapped(int x, int y, int size, Uint32 color ) {
	int R = getRed(color);
	int G = getGreen(color);
	int B = getBlue(color);

	filledCircleRGBA( main, x, y, size + 2, 0, 0, 0, 128 );
	filledCircleRGBA( main, x, y, size, R, G, B, 255 );
	aacircleRGBA( main, x, y, size, R, G, B, 255 );
	filledEllipseRGBA( main, x, y - size / 2, size, size / 2, 255 - 4 * ( 255 - R ) / 5, 255 - 4 * ( 255 - G ) / 5, 255 - 4 * ( 255 - B ) / 5, 255 );
	aaellipseRGBA( main, x, y - size / 2, size, size / 2, 255 - 4 * ( 255 - R ) / 5, 255 - 4 * ( 255 - G ) / 5, 255 - 4 * ( 255 - B ) / 5, 255 );
}

void
Canvas::drawResidentShip(Coordinate& shipLocation, Coordinate& planetLocation,
			int color) {
	int sx = shipLocation.getXMapped();
	int sy = shipLocation.getYMapped();

	int px = planetLocation.getXMapped();
	int py = planetLocation.getYMapped();

	int r = getRed(color);
	int g = getGreen(color);
	int b = getBlue(color);

	aalineRGBA(main, sx, sy, px, py, r, g, b, 64);
        filledCircleRGBA(main, sx, sy, 1, r, g, b, 255);
}

void
Canvas::drawFlyingShip(Coordinate& shipLocation, double direction, int color)
{
	int sx = shipLocation.getXMapped();
	int sy = shipLocation.getYMapped();

	int r = getRed(color);
	int g = getGreen(color);
	int b = getBlue(color);

        int headX, headY;
        int leftTipX, leftTipY;
        int rightTipX, rightTipY;

        headX = (int) (sx + 5.0 * cos( direction));
	headY = (int) (sy + 5.0 * sin( direction));
	leftTipX = (int) (sx + 2.0 * cos( direction + M_PI / 3.0));
	leftTipY = (int) (sy + 2.0 * sin( direction + M_PI / 3.0));
	rightTipX = (int) (sx + 2.0 * cos( direction - M_PI / 3.0));
	rightTipY = (int) (sy + 2.0 * sin( direction - M_PI / 3.0));

        aalineRGBA(main, leftTipX, leftTipY, headX, headY, r, g, b, 255);
        aalineRGBA(main, rightTipX, rightTipY, headX, headY, r, g, b, 255);
        aalineRGBA(main, rightTipX, rightTipY, leftTipX, leftTipY, r, g, b, 255);
}

void
Canvas::drawSelection(Coordinate& location)
{
	aacircleRGBA(main, location.getXMapped(), location.getYMapped(), 3, 255, 192, 0, 255 );
}

void
Canvas::drawOrbit(Coordinate &center, double rotationDistance, int color)
{
	int r = getRed(color);
	int g = getGreen(color);
	int b = getBlue(color);

	aaellipseRGBA(main, center.getXMapped(), center.getYMapped(),
		      (int) (rotationDistance * Settings::getGameWidth()),
                      (int) (rotationDistance * Settings::getGameHeight()),
		      r, g, b, 64);
}

void
Canvas::drawBuildProgress(Coordinate& location, int size, double percentage)
{
	aacircleRGBA( main, location.getXMapped(), location.getYMapped(),
		      (int) (size + 102.5 - percentage),
		      0xff, 0xff, 0xff,
		      (int)( 0.2 * percentage ));
}

SDL_Surface *
Canvas::initScreen()
{
	long flags = SDL_SWSURFACE;
	if (Settings::getFullscreen())
          flags |= SDL_FULLSCREEN;

	return main = SDL_SetVideoMode( Settings::getScreenWidth(), Settings::getScreenHeight(), 32, flags);
}

