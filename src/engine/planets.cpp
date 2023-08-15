// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include <math.h>

#include "planets.h"
#include "timer.h"
#include "ships.h"
#include "animations.h"
#include "coordinate.h"
#include "selection.h"
#include "players.h"
#include "universe.h"
#include "messages.h"
#include "canvas.h"

using namespace std;

// ##### PLANET #####

Planet::Planet() : nearestPlanet(false), owner(), mother()
{
	rotationSpeed = ( ( rand() % 2 ) * 2 - 1 ) * ( rand() % 60000 + 20000 );
	rotationDistance = 0.05 + 0.2 * ( double )rand() / RAND_MAX;
	rotationOffset = rand() % 10000000;
	size = 4 + rand() % 4; // [4 ... 8)
	isAMoon = false;
	selected = false;
	sourceSelected = false;
	shipCreationSpeed = 12500 - size*1000;
  
  double angle = rotationOffset / rotationSpeed;
  locationVector.setX(cos(angle));
  locationVector.setY(sin(angle));
  
  buildStartTime = 0;
  buildEndTime = shipCreationSpeed;
}

void
Planet::makeMoon( Planet* mother ) {
	rotationSpeed = ( ( rand() % 2 ) * 2 - 1 ) * ( rand() % 2000 + 2000 );
	rotationDistance = 0.01 + 0.03 * ( double )rand() / RAND_MAX;
	rotationOffset = rand() % 100000;
	size = 2 + rand() % 2; // [2 ... 4)
	isAMoon = true;
	this->mother = mother;
	shipCreationSpeed = 15000 - size*1000;
}

void
Planet::update(Uint32 time)
{
  double angle = (double)(rotationOffset + time) / rotationSpeed;
  locationVector.setX(cos(angle));
  locationVector.setY(sin(angle));
  
  if (buildEndTime <= time)
  {
    createShip(time);
    buildStartTime = time;
    buildEndTime = time + shipCreationSpeed;
  }
  
  if( residentShips.size() > 0 ) {
    Coordinate location = getLocation();
    int counter = 0;
    double offset = 500 * rotationDistance + (double) time / 10000;
    for( list< Ship* >::iterator i = residentShips.begin(); i != residentShips.end(); i++ ) {
      double shipX = location.getX() + 0.02 * cos( offset + counter * 2 * M_PI / residentShips.size() );
      double shipY = location.getY() + 0.02 * sin( offset + counter * 2 * M_PI / residentShips.size() );
      (*i)->setLocation( shipX, shipY );
      counter++;
    }
  }
  
}

Coordinate
Planet::getLocation() const {
	Coordinate motherLocation;
	if( isAMoon ) {
		motherLocation = mother->getLocation();
	} else {
		motherLocation.setX( 0.5 );
		motherLocation.setY( 0.5 );
	}

	double x = motherLocation.getX() + rotationDistance * locationVector.getX();
	double y = motherLocation.getY() + rotationDistance * locationVector.getY();
	return Coordinate( x, y );
}

void
Planet::addResident( Ship* ship, Uint32 time )
{
  if (owner != ship->getOwner())
  {
    // Planet is conquered.
    if (residentShips.size() == 0)
    {
      setOwner(ship->getOwner());

      // Plays correspoing animation.
      // TODO: This should be triggered through player->removePlanet()/addPlanet()
      Uint32 color = owner->getColor();
      universe->animationQueue->scheduleAction(time + 1200,
        new SonarAnimation(this,
                           color,
                           isAMoon ? 50 : 100,
                           time,
                           time + 1000,
                           true));
                           
      residentShips.push_back( ship );
    }
    else
    {
		  ship->die();
		  Ship* resident = *( residentShips.begin() );
		  removeResident( resident );
		  resident->die();
		
		  // show an animation of die'ing ships that are of our interest
		  if( ( ship->getOwner()->getPlayerType() == Player::HUMAN ) || ( owner->getPlayerType() == Player::HUMAN ) )
			 universe->animationQueue->scheduleAction( time + 1000, new SonarAnimation( this, ship->getOwner()->getColor(), 20, time, time + 1000, true ) );
    }
    
  }
  else
  {
    residentShips.push_back( ship );
	}
  
}

void
Planet::setOwner(Player *newOwner)
{
  if (owner == newOwner)
    return;
  
  if (owner != NULL)
    owner->removePlanet(this);

  newOwner->addPlanet(this);
  owner = newOwner;
 
}

void
Planet::removeResident( Ship* ship ) {
	residentShips.remove( ship );
}

void
Planet::render(Uint32 time) const {
	if( owner->getPlayerType() == Player::HUMAN )
		renderBuildProgress(time);
	Coordinate location = getLocation();

	Canvas::drawPlanet(location, size, owner->getColor() );
	if( selected )
        {
		Canvas::drawSelector(location, -4, 10, 10, 0, 0, 0);
		Canvas::drawSelector(location, -5, 10, 10, 255, 192, 0);
        }

	if( sourceSelected )
        {
		Canvas::drawSelector(location, -4, 10, 10, 0, 0, 0);
		Canvas::drawSelector(location, -5, 10, 10, 255, 255, 0);
        }

	if (nearestPlanet)
	{
		Canvas::drawNearestPlanetSelector(location, size);
	}
}

void
Planet::renderOrbit( ) const {
	Coordinate centerLocation;
	if( isAMoon ) {
		centerLocation = mother->getLocation();
	} else {
		centerLocation.setX( 0.5 );
		centerLocation.setY( 0.5 );
	}
	if( owner != NULL )
		Canvas::drawOrbit(centerLocation, rotationDistance, owner->getColor());
	if( getMoon() ) {
		mother->renderOrbit( );
	}
}

void
Planet::renderBuildProgress(Uint32 time) const {

	Coordinate location = getLocation();
	double percentage = 100.0 * ( time - buildStartTime ) / ( buildEndTime - buildStartTime );

	Canvas::drawBuildProgress(location, size, percentage);
}

double
Planet::distance( Planet* planet ) {
	return planet->getLocation().distance( getLocation() );
}

void
Planet::setNearestPlanet(bool b) {
	nearestPlanet = b;
}

void
Planet::setSourceSelected( bool selected ) {
	sourceSelected = selected;
}

void
Planet::setSelected( bool selected ) {
	this->selected = selected;
}

void
Planet::setUniverse( Universe* universe ) {
	this->universe = universe;
}

void
Planet::createShip(const Uint32& time) {
	if( owner && owner->getPlayerType() != Player::NEUTRAL )
    owner->addShip(time, this);
}

void
Planet::moveResidentsTo(Uint32 time, Planet *destination, int fleetSelection) {
  // Save same fuel ... :)
  if (destination == this || !residentShips.size()) {
    return;
  }
    
  // fleetSelection of 1 means "send one ship". Otherwise it means
  //  "send fleetSelection percent of the available ships.".
  int decampeeCount = (fleetSelection == 1)
                      ? ((residentShips.size() > 0) ? 1 : 0)
                      : (fleetSelection * residentShips.size()) / 100;
  if (!decampeeCount)
    return;
  
  // Calling Ship->moveTo will affect the residentShips list so we have to
  // copy the ships which are going for war into their own list.
  list <Ship *> decampees;
  for (list <Ship *>::iterator i = residentShips.begin(); decampeeCount > 0; i++) {
    decampees.push_back(*i);
    decampeeCount--;
  }
  
  for (list <Ship *>::iterator i = decampees.begin(); i != decampees.end(); i++) {
    (*i)->moveTo(time + rand() % 500, destination, universe->actionQueue);
  }
}

// ##### PLANETS #####

Planets::Planets() {
}

Planets::Planets( int numberOfPlanets, int numberOfMoons ) {

	addPlanets( numberOfPlanets );
	addMoons( numberOfMoons );
	
}

Planets::~Planets()
{
  for (list < Planet *>::iterator i = begin(); i != end(); i++)
  {
      delete *i;
  }
}

void
Planets::addPlanets( int numberOfPlanets ) {

	for( int i = 0; i < numberOfPlanets; i++ ) {
		push_back( new Planet() );
	}
	
}

void
Planets::addMoons( int numberOfMoons ) {

	if( size() == 0 ) {
		cerr << "Error constructing moons, no planets yet" << endl;
		exit( 1 );
	}

	for( int i = 0; i < numberOfMoons; i++ ) {
		iterator motherPlanet;
		bool done = false;
		
    do {
			int mother = rand() % size();
			motherPlanet = begin();
			for( int j = 0; j < mother; j++ ) motherPlanet++;
			 done = !(*motherPlanet)->getMoon();
		} while( !done );
    
		Planet *p = new Planet();
		p->makeMoon(*motherPlanet);
		push_back( p );
	}
	
}

void 
Planets::render(Uint32 time) const {
	for( const_iterator i = begin(); i != end(); i++ )
		(*i)->render(time);
}

void 
Planets::renderOrbits( ) const {
	for( const_iterator i = begin(); i != end(); i++ )
		(*i)->renderOrbit( );
}

Planet*
Planets::closestToCoordinate( const Coordinate& c ) {
	double closestDistance = 5000;
	Planet* closestPlanet = NULL;
	for( iterator i = begin(); i != end(); i++ ) {
		double distance = (*i)->getLocation().distance( c );
		if( distance < closestDistance ) {
			closestDistance = distance;
			closestPlanet = (*i);
		}
	}
	return closestPlanet;
}

Planet*
Planets::closestToCoordinate( const Coordinate& c, double treshold ) {
	Planet* closest = closestToCoordinate( c );
	if( closest->getLocation().distance( c ) < treshold )
		return closest;
	else
		return NULL;
}

void
Planets::select( Selection selection ) {

	double minX = selection.getMinX();
	double maxX = selection.getMaxX();
	double minY = selection.getMinY();
	double maxY = selection.getMaxY();
	
	for( iterator i = begin(); i != end(); i++ ) {
		Coordinate location = (*i)->getLocation();
		if( ( location.getX() > minX ) && ( location.getX() < maxX ) && ( location.getY() > minY ) && ( location.getY() < maxY ) )
			(*i)->setSelected( true );
		else
			(*i)->setSelected( false );
	}
	
}

void
Planets::sourceSelect( Selection *selection, Player *owner ) {

	double minX = selection->getMinX();
	double maxX = selection->getMaxX();
	double minY = selection->getMinY();
	double maxY = selection->getMaxY();
	
	for( iterator i = begin(); i != end(); i++ ) {
		Coordinate location = (*i)->getLocation();
		if( ( location.getX() > minX ) && ( location.getX() < maxX )
		      && ( location.getY() > minY ) && ( location.getY() < maxY )
		      && (*i)->getOwner() == owner )
			(*i)->setSourceSelected( true );
		else
			(*i)->setSourceSelected( false );
	}
	
}

void 
Planets::update(Uint32 time) {
	for( iterator i = begin(); i != end(); i++ )
		(*i)->update(time);
}

void
Planets::setUniverse( Universe* universe ) {
	for( iterator i = begin(); i != end(); i++ )
		(*i)->setUniverse( universe );
}

Planet*
Planets::getRandomPlanet() {
	int number = rand() % size();
	iterator i = begin();
	for( int k = 0; k < number; k++ )
		i++;
	return (*i);
}

Planet*
Planets::getRandomEnemyPlanet( Player* player ) {
  // TODO: This has to be implemented
  return nullptr;
}

Planet*
Planets::getRandomNearbyPlanet( Planet* planet ) {

	Planet* result;
	do {
		result = getRandomPlanet();
	} while( result == planet );
	double distance = planet->distance( result );
	
	for( int i = 0; i < size() / 2; i++ ) {
		Planet* otherPlanet;
		do {
			otherPlanet = getRandomPlanet();
		} while( otherPlanet == planet );
		double otherDistance = planet->distance( otherPlanet );
		
		// which planets do we prefer?
		
		if( otherPlanet->getOwner() != planet->getOwner() )
			otherDistance *= 0.1;
		if( otherPlanet->getOwner()->getPlayerType() == Player::NEUTRAL )
			otherDistance *= 0.5;
		if( ! otherPlanet->getMoon() )
			otherDistance *= 0.8;
		
		if( otherDistance < distance ) {
			result = otherPlanet;
			distance = otherDistance;
		}
	}
	
	return result;

}

// ##### CREATESHIPACTION #####
CreateShipAction::CreateShipAction(Planet *planet )
{
	this->planet = planet;
	actionID = 1;
}

void
CreateShipAction::execute( const Uint32& time ) {
	planet->createShip(time);
}

