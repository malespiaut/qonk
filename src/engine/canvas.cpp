// Copyright 2005 by Anthony Liekens <anthony@liekens.net>
// Copyright 2007 by Robert Schuster <robertschuster@fsfe.org>

#include <SDL/SDL_gfxPrimitives.h>

#include "canvas.h"
#include "coordinate.h"
#include "extensions.h"

#include "../fonts.h"

SDL_Surface* Canvas::main = 0;

gcn::SDLGraphics *Canvas::sdlGraphics = 0;
int Canvas::width = 0;
int Canvas::height = 0;
int Canvas::gameOffsetX = 0;

Font* Canvas::font = 0;

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
Canvas::drawPlanet( Coordinate &loc, int size, Uint32 color )
{
	drawPlanetMapped(loc.getXMapped(), loc.getYMapped(), size, color);
}

void
Canvas::drawPlanetMapped(int x, int y, int size, Uint32 color )
{
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
		      (int) (rotationDistance * height),
                      (int) (rotationDistance * height),
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

void
Canvas::drawText(int x, int y, const char *msg, int r, int g, int b, int a)
{
    font->render(main, x, y, msg, r, g, b, a);
}

void
Canvas::drawBox(int x, int y, int w, int h, int r, int g, int b)
{
  boxRGBA( main, x, y, w, h, r, g, b, 255 );
}

int
Canvas::getFontHeight()
{
  return font->getHeight();
}

void
Canvas::drawRadar()
{
    int radarSteps = 4;

    double s = 1.0 / radarSteps;
    for( int i = 1; i <= radarSteps; i++ ) {
        aaellipseRGBA( main, gameOffsetX + height / 2,
                height / 2, (int) (i * s * height / 2),
                (int)( i * s * height / 2 ), 144, 225, 144, 64 );
    }
    lineRGBA( main,
          gameOffsetX + height / 2, 0,
          gameOffsetX + height / 2,
          height, 255, 225, 144, 64 );
    lineRGBA( main,
          gameOffsetX,
          height / 2,
          gameOffsetX + height,
          height / 2, 255, 225, 144, 64 );
}

void
Canvas::drawSun()
{
    // Sun in the middle
    Sint16 x0 = gameOffsetX + height / 2;
    Sint16 y0 = height / 2;

    filledTrigonRGBA( main, x0+5, y0, x0-5,y0, x0, y0+15, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0+5, y0, x0-5,y0, x0, y0-15, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0+15, y0, x0,y0+5, x0, y0-5, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0-15, y0, x0,y0+5, x0, y0-5, 255, 205, 0, 255);

    filledTrigonRGBA( main, x0+10, y0-10, x0+5,y0, x0, y0-5, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0-10, y0-10, x0-5,y0, x0, y0-5, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0+10, y0+10, x0+5,y0, x0, y0+5, 255, 205, 0, 255);
    filledTrigonRGBA( main, x0-10, y0+10, x0-5,y0, x0, y0+5, 255, 205, 0, 255);

    filledCircleRGBA( main, x0,y0, 7, 255, 255, 0, 255 );
    filledCircleRGBA( main, x0,y0, 6, 255, 245, 0, 255 );
    filledCircleRGBA( main, x0,y0, 5, 255, 225, 0, 255 );
    filledCircleRGBA( main, x0,y0, 4, 255, 205, 0, 255 );
}

/** Little star drawn in the background.
 *
 */
void
Canvas::drawStar(int x, int y, int brightness)
{
  pixelRGBA( main, x, y, 
               brightness + rand() % 64, 
               brightness + rand() % 64, 
               brightness + rand() % 64, 
               255 );

}

void
Canvas::drawSonar(Coordinate coord, int size, double percentage, int r, int g, int b, bool circle)
{
  int x = coord.getXMapped();
  int y = coord.getYMapped();

  filledCircleRGBA( main, x, y, (int)( size * sin( percentage * M_PI ) ), r, g, b, (int)( ( 255 - 255 * percentage ) * 0.05 ) );

  if( circle )
    aacircleRGBA( main, x, y, (int)( size * sin( percentage * M_PI ) ), r, g, b, 255 - (int)( 255 * percentage ) );

}

void
Canvas::drawPlayerStat(int size, int index, int previousValue, int currentValue, int r, int g, int b)
{
  aalineRGBA( main, width - size + index - 1, height - previousValue,
                    width - size + index, height - currentValue,
                    r, g, b,  index * 2 );
}

void
Canvas::drawMouseSelection(Coordinate &c1, Coordinate &c2)
{
  rectangleColor( main, c1.getXMapped(), c1.getYMapped(),
                        c2.getXMapped(), c2.getYMapped(), 0xfee19080 );
}

void
Canvas::drawCursor(Coordinate &c)
{
    aacircleRGBA( main, c.getXMapped(), c.getYMapped(), 4, 255, 255, 255, 255 );
}

void
Canvas::initScreen(int w, int h, bool fullscreen)
{
  if (!sdlGraphics)
    sdlGraphics = new gcn::SDLGraphics();

  if (!font)
     font = new Font("font.ttf", 18);

	long flags = SDL_SWSURFACE | SDL_HWSURFACE;
  
	if (fullscreen)
    flags |= SDL_FULLSCREEN;
          
  width = w;
  height = h;
  gameOffsetX = ( w - h ) / 2;
  
	main = SDL_SetVideoMode( w, h, 0, flags);
  sdlGraphics->setTarget(main);
}

int
Canvas::getWidth()
{
  return width;
}

int
Canvas::getHeight()
{
  return height;
}

void
Canvas::shutdown()
{
  delete sdlGraphics;
  SDL_FreeSurface(main);
  main = NULL;
}

void
Canvas::updateScreen()
{
  SDL_Flip(main);
}

gcn::SDLGraphics *
Canvas::getSDLGraphics()
{
  return sdlGraphics;
}
