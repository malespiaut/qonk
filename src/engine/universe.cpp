// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include <iostream>

#include <SDL/SDL_gfxPrimitives.h>

#include "universe.h"
#include "planets.h"
#include "players.h"
#include "stars.h"
#include "actions.h"
#include "animations.h"
#include "timer.h"
#include "coordinate.h"

#include "canvas.h"

Universe::Universe() : currentSelectedPlanet(0) {
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
  
  stars = new Stars( 100 );
  planets = new Planets( 15, 15 );
  planets->setUniverse( this );
}

Universe::Universe( int nrPlanets ) : currentSelectedPlanet(0) {
  actionQueue = new ActionQueue();
  animationQueue = new AnimationQueue();
  
  stars = new Stars( 100 );
  planets = new Planets( nrPlanets, nrPlanets );
  planets->setUniverse( this );
}

Universe::~Universe() {
	delete stars;
	delete planets;
	delete actionQueue;
	delete animationQueue;
}

void
Universe::update(Uint32 time) {
  actionQueue->executeEventsBefore( time );
  animationQueue->executeEventsBefore( time );
  
  planets->update(time);
}

void
Universe::highlightNearestPlanet(Coordinate &c)
{
	Planet* closestPlanet = planets->closestToCoordinate( c, 1 );

  if (currentSelectedPlanet)
	  currentSelectedPlanet->setNearestPlanet(false);
		
	currentSelectedPlanet = closestPlanet;

	if (currentSelectedPlanet)
	  currentSelectedPlanet->setNearestPlanet(true);

}

void
Universe::renderBackground(Uint32 time) {

	stars->render( );
	
  Canvas::drawRadar();
	
	animationQueue->render(time);

  Canvas::drawSun();
}

void
Universe::renderForeground(Uint32 time) {
	planets->render(time);
}

/* Assigns unclaimed planets and moons to players.
 * 
 * The algorithm tries to claim as many planets as
 * given by planetCount. However if there are not
 * enough unclaimed planets to fullfil the request
 * a lower number of planets will be claimed.
 * 
 * Each planet is given shipCount ships.  
 * 
 * Planets are claimed even if no ship is placed on
 * it!
 */
void
Universe::claim(Uint32 time, Player *player, int planetCount, int shipCount)
{
  Planets::iterator i = planets->begin();

  while (i != planets->end())
  {
    if (!((*i)->getMoon() || (*i)->getOwner()))
    {
      Planet *p = (*i);
      
      p->setOwner(player);
      
      for (int s = 0; s < shipCount; s++)
        player->addShip(time, p);
        
      if (--planetCount == 0)
        return;
    }
    
    i++;
  }
  
}

void
Universe::claimRemaining(Uint32 time, Player *player, int maxPlanetShipCount, int maxMoonShipCount)
{
  Planets::iterator i = planets->begin();

  while (i != planets->end())
  {
    if (!(*i)->getOwner())
    {
      Planet *p = (*i);

      p->setOwner(player);
      
      int ships = 1 + rand() % (p->getMoon() ? maxMoonShipCount : maxPlanetShipCount);
      
      for (; ships > 0; ships--)
        player->addShip(time, p);
        
    }
    i++;
  }
}
