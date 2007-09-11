// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "fonts.h"

#include "canvas.h"

using namespace std;

Font::Font( char* fileName, int size ) {
  std::string s = PKG_DATA_DIR;
  s += fileName;

  if (font = TTF_OpenFont (s.c_str(), size)) {
    TTF_SetFontStyle( font, TTF_STYLE_NORMAL );
    return;
  }

  s = "data";
#ifdef WINDOWS
  s += "\\";
#else
  s += "/";
#endif
  s += fileName;

  if (font = TTF_OpenFont (s.c_str(), size)) {
    TTF_SetFontStyle( font, TTF_STYLE_NORMAL );
    return;
  }
  
  cerr << "Could not load " << size << "pt font " << fileName << " from the following directories: " << endl
       << PKG_DATA_DIR << endl
       << "data" << endl;
  exit( 1 );
}

void
Font::render( SDL_Surface* screen, int x, int y, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
  SDL_Color color = { r, g, b, a };
  SDL_Surface* textSurface = TTF_RenderText_Blended( font, text, color );
	if ( textSurface != NULL ) {
		SDL_Rect dstrect;
		dstrect.x = x;
		dstrect.y = y;
		dstrect.w = textSurface->w;
		dstrect.h = textSurface->h;
		Canvas::alphaBlend( textSurface, a );
		SDL_BlitSurface( textSurface, NULL, screen, &dstrect );
		SDL_FreeSurface( textSurface );
	}
}

void
Font::renderCenterJustified( SDL_Surface* screen, int x, int y, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
  SDL_Color color = { r, g, b, 0 };
  SDL_Surface* textSurface = TTF_RenderText_Blended( font, text, color );
	if ( textSurface != NULL ) {
		SDL_Rect dstrect;
		dstrect.x = x - textSurface->w / 2;
		dstrect.y = y - textSurface->h / 2;
		dstrect.w = textSurface->w;
		dstrect.h = textSurface->h;
		Canvas::alphaBlend( textSurface, a );
		SDL_BlitSurface( textSurface, NULL, screen, &dstrect );
		SDL_FreeSurface( textSurface );
	}
}

void
Font::renderRightJustified( SDL_Surface* screen, int x, int y, const char* text, Uint8 r, Uint8 g, Uint8 b, Uint8 a ) {
  SDL_Color color = { r, g, b, 0 };
  SDL_Surface* textSurface = TTF_RenderText_Blended( font, text, color );
	if ( textSurface != NULL ) {
		SDL_Rect dstrect;
		dstrect.x = x - textSurface->w;
		dstrect.y = y;
		dstrect.w = textSurface->w;
		dstrect.h = textSurface->h;
		Canvas::alphaBlend( textSurface, a );
		SDL_BlitSurface( textSurface, NULL, screen, &dstrect );
		SDL_FreeSurface( textSurface );
	}
}

int
Font::getHeight() {
	return TTF_FontHeight( font );
}
