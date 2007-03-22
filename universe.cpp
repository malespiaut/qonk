// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "universe.h"
#include <SDL_gfxPrimitives.h>
#include <iostream>

using namespace std;

Universe::Universe() {
  stars = new Stars( 100 );
  planets = new Planets( 15, 15 );
  planets->setUniverse( this );
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
  messages = new Messages();
}

Universe::Universe( int nrPlanets ) {
  stars = new Stars( 100 );
  planets = new Planets( nrPlanets, nrPlanets );
  planets->setUniverse( this );
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
  messages = new Messages();
}

Universe::~Universe() {
	delete stars;
	delete planets;
	delete actionQueue;
	delete animationQueue;
	delete messages;
}

void
Universe::update() {
  actionQueue->executeEventsBefore( timer.getTime() );
  animationQueue->executeEventsBefore( timer.getTime() );
}

void
Universe::renderBackground( SDL_Surface* screen ) {

	stars->render( screen );
	
	messages->render( screen );
	
	int radarSteps = 4;
	Uint32 radarColor = 0xfee190;
	double s = 1.0 / radarSteps;
	for( int i = 1; i <= radarSteps; i++ ) {
		aaellipseRGBA( screen, settings.getGameOffsetX() + settings.getGameWidth() / 2, settings.getGameHeight() / 2, (int)( i * s * settings.getGameWidth() / 2 ), (int)( i * s * settings.getGameHeight() / 2 ), 144, 225, 144, 64 );
	}
	lineRGBA( screen, settings.getGameOffsetX() + settings.getGameWidth() / 2, 0, settings.getGameOffsetX() + settings.getGameWidth() / 2, settings.getGameHeight(), 255, 225, 144, 64 );
	lineRGBA( screen, settings.getGameOffsetX(), settings.getGameHeight() / 2, settings.getGameOffsetX() + settings.getGameWidth(), settings.getGameHeight() / 2, 255, 225, 144, 64 );
	
	int mouseX;
	int mouseY;
	Uint8 mouseState = SDL_GetMouseState( &mouseX, &mouseY );
	double pointerX = ( (double)mouseX - settings.getGameOffsetX() ) / settings.getGameWidth();
	double pointerY = (double)mouseY / settings.getGameHeight();
	Planet* closestPlanet = planets->closestToCoordinate( Coordinate( pointerX, pointerY ), 1 );

	if( !( mouseState & SDL_BUTTON(1) ) ) {

//		if( closestPlanet != NULL ) {
//			closestPlanet->renderOrbit( screen );
//		}
		
		currentSelectedPlanet = closestPlanet;
			
	}
	
	animationQueue->render( screen );


// Sun in the middle
	Sint16 x0 = settings.getGameOffsetX() + settings.getGameWidth() / 2;
	Sint16 y0 =	settings.getGameHeight() / 2;
	
	filledTrigonRGBA( screen, x0+5, y0, x0-5,y0, x0, y0+15, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0+5, y0, x0-5,y0, x0, y0-15, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0+15, y0, x0,y0+5, x0, y0-5, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0-15, y0, x0,y0+5, x0, y0-5, 255, 205, 0, 255);
	
	filledTrigonRGBA( screen, x0+10, y0-10, x0+5,y0, x0, y0-5, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0-10, y0-10, x0-5,y0, x0, y0-5, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0+10, y0+10, x0+5,y0, x0, y0+5, 255, 205, 0, 255);
	filledTrigonRGBA( screen, x0-10, y0+10, x0-5,y0, x0, y0+5, 255, 205, 0, 255);
	
	filledCircleRGBA( screen, x0,y0, 7, 255, 255, 0, 255 );
	filledCircleRGBA( screen, x0,y0, 6, 255, 245, 0, 255 );
	filledCircleRGBA( screen, x0,y0, 5, 255, 225, 0, 255 );
	filledCircleRGBA( screen, x0,y0, 4, 255, 205, 0, 255 );
}

void
Universe::renderForeground( SDL_Surface* screen ) {

	planets->updateShipLocations();
	planets->render( screen );

	if( currentSelectedPlanet != NULL ) {
		currentSelectedPlanet->renderSelector( screen );
	}
	
}

