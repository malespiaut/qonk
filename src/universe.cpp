// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include <iostream>

#include <SDL/SDL_gfxPrimitives.h>

#include "universe.h"
#include "planets.h"
#include "stars.h"
#include "actions.h"
#include "animations.h"
#include "timer.h"
#include "settings.h"
#include "canvas.h"

Universe::Universe() : currentSelectedPlanet(0) {
  stars = new Stars( 100 );
  planets = new Planets( 15, 15 );
  planets->setUniverse( this );
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
}

Universe::Universe( int nrPlanets ) : currentSelectedPlanet(0) {
  stars = new Stars( 100 );
  planets = new Planets( nrPlanets, nrPlanets );
  planets->setUniverse( this );
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
}

Universe::~Universe() {
	delete stars;
	delete planets;
	delete actionQueue;
	delete animationQueue;
}

void
Universe::update() {
  actionQueue->executeEventsBefore( timer.getTime() );
  animationQueue->executeEventsBefore( timer.getTime() );
}

void
Universe::highlightNearestPlanet(int x, int y)
{
	double pointerX = ( (double)x - Settings::getGameOffsetX() ) / Settings::getGameWidth();
	double pointerY = (double)y / Settings::getGameHeight();
	Planet* closestPlanet = planets->closestToCoordinate( Coordinate( pointerX, pointerY ), 1 );

  if (currentSelectedPlanet)
	  currentSelectedPlanet->setNearestPlanet(false);
		
	currentSelectedPlanet = closestPlanet;

	if (currentSelectedPlanet)
	  currentSelectedPlanet->setNearestPlanet(true);

}

void
Universe::renderBackground() {

	stars->render( );
	
  Canvas::drawRadar();
	
	animationQueue->render();

  Canvas::drawSun();
}

void
Universe::renderForeground( ) {
	planets->updateShipLocations();
	planets->render( );
}
