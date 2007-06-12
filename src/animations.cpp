// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include <SDL/SDL_gfxPrimitives.h>

#include "animations.h"
#include "timer.h"
#include "planets.h"
#include "coordinate.h"


void
AnimationQueue::render( SDL_Surface* screen ) {
  for( iterator i = begin(); i != end(); i++ ) {
    Animation* animation = (Animation*)( i->second );
    animation->render( screen );
  }
}

SonarAnimation::SonarAnimation() {
}

SonarAnimation::SonarAnimation( Planet* planet, const Uint32& color, const int& size, const Uint32& startTime, const Uint32& endTime, bool circle ) {
  this->circle = circle;
  this->planet = planet;
  r = ( color & 0xFF0000 ) >> 16;
  g = ( color & 0x00FF00 ) >> 8;
  b = ( color & 0x0000FF );
//  this->color = color;
  this->size = size;
  this->startTime = startTime;
  this->endTime = endTime;
}

void
SonarAnimation::render( SDL_Surface* screen ) {
  Uint32 currentTime = timer.getTime();
  if( ( currentTime > startTime ) && ( currentTime < endTime ) ) {
    double percentage = (double)( currentTime - startTime ) / ( endTime - startTime );
    filledCircleRGBA( screen, planet->getLocation().getXMapped(), planet->getLocation().getYMapped(), (int)( size * sin( percentage * M_PI ) ), r, g, b, (int)( ( 255 - 255 * percentage ) * 0.05 ) );
    if( circle )
      aacircleRGBA( screen, planet->getLocation().getXMapped(), planet->getLocation().getYMapped(), (int)( size * sin( percentage * M_PI ) ), r, g, b, 255 - (int)( 255 * percentage ) );
  }
}
