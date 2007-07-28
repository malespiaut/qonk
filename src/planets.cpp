// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include <math.h>

#include "planets.h"
#include "timer.h"
#include "ships.h"
#include "extensions.h"
#include "settings.h"
#include "animations.h"
#include "coordinate.h"
#include "selection.h"
#include "players.h"
#include "universe.h"
#include "messages.h"
#include "canvas.h"

using namespace std;

// ##### PLANET #####

Planet::Planet() : nearestPlanet(false) {
	rotationSpeed = ( ( rand() % 2 ) * 2 - 1 ) * ( rand() % 60000 + 20000 );
	rotationDistance = 0.05 + 0.4 * ( double )rand() / RAND_MAX;
	rotationOffset = rand() % 10000000;
	size = 4 + rand() % 4; // [4 ... 8)
	isAMoon = false;
	mother = NULL;
	selected = false;
	sourceSelected = false;
	owner = NULL;
	shipCreationSpeed = 12500 - size*1000;
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

Coordinate
Planet::getVector( Uint32 time ) const {
	double angle = (double)( rotationOffset + time ) / rotationSpeed;
	return Coordinate( cos( angle ), sin( angle ) );
}

Coordinate
Planet::getLocation( Uint32 time ) const {
	Coordinate motherLocation;
	if( isAMoon ) {
		motherLocation = mother->getLocation();
	} else {
		motherLocation.setX( 0.5 );
		motherLocation.setY( 0.5 );
	}

	Coordinate v = getVector( time );
	double x = motherLocation.getX() + rotationDistance * v.getX();
	double y = motherLocation.getY() + rotationDistance * v.getY();
	return Coordinate( x, y );
}

Coordinate
Planet::getLocation() const {
	return getLocation( timer.getTime() );
}

void
Planet::addResident( Ship* ship, Uint32 time ) {
	if( ( owner == NULL ) || ( owner == ship->getOwner() ) || ( residentShips.size() == 0 ) ) {
		if( owner != ship->getOwner() ) {
		
			// clear out "create ship" actions scheduled for this planet
			vector< ActionQueue::iterator > removes;
			for( ActionQueue::iterator i = universe->actionQueue->begin(); i != universe->actionQueue->end(); i++ ) {
				if( (i->second)->getActionID() == 1 ) {
					Planet* planetOfIterator = ((CreateShipAction*)(i->second))->getPlanet();
					if( planetOfIterator == this )
						removes.push_back( i );
				}
			}
			
			for( vector< ActionQueue::iterator >::iterator i = removes.begin(); i != removes.end(); i++ ) {
				universe->actionQueue->erase( *i );
			}
			
			// create a new "create ship" action
			buildStartTime = time;
			buildEndTime = time + shipCreationSpeed;
	    universe->actionQueue->scheduleAction( buildEndTime, new CreateShipAction( ship->getOwner(), this ) );
	    
			// show an animation of planet conquests that are of our interest
			Uint32 color = 0;
			color = ship->getOwner()->getColor();
			if( owner != NULL )
			if( isAMoon )
				universe->animationQueue->scheduleAction( time + 1200, new SonarAnimation( this, color, 50, time, time + 1000, true ) );
			else
				universe->animationQueue->scheduleAction( time + 1200, new SonarAnimation( this, color, 100, time, time + 1000, true ) );
			
    }
		residentShips.push_back( ship );
		setOwner(ship->getOwner());
		
	} else {
		ship->die();
		Ship* resident = *( residentShips.begin() );
		removeResident( resident );
		resident->die();
		
		// show an animation of die'ing ships that are of our interest
		if( ( ship->getOwner()->getPlayerType() == Player::HUMAN ) || ( owner->getPlayerType() == Player::HUMAN ) ) {
			universe->animationQueue->scheduleAction( time + 1000, new SonarAnimation( this, ship->getOwner()->getColor(), 20, time, time + 1000, true ) );
		}
//		if( owner->getPlayerType() == Player::HUMAN ) {
//			universe->animationQueue->scheduleAction( time + 1000, new SonarAnimation( this, 0xc00000, 20, time, time + 1000 ) );
//		}
	}
}

void
Planet::setOwner(Player *newOwner)
{
 if (owner != NULL)
   owner->removePlanet(this);
   
 owner = newOwner;
 
 newOwner->addPlanet(this);
}

void
Planet::removeResident( Ship* ship ) {
	residentShips.remove( ship );
}

void
Planet::render( ) const {
	if( owner->getPlayerType() == Player::HUMAN )
		renderBuildProgress( );
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
Planet::renderBuildProgress( ) const {

	Coordinate location = getLocation();
	int x = location.getXMapped();
	int y = location.getYMapped();
	double percentage = 100.0 * ( timer.getTime() - buildStartTime ) / ( buildEndTime - buildStartTime );

	Canvas::drawBuildProgress(location, size, percentage);
}

void
Planet::updateShipLocations() {
	if( residentShips.size() > 0 ) {
		Coordinate location = getLocation();
		int counter = 0;
		double offset = 500 * rotationDistance + (double)timer.getTime() / 10000;
		for( list< Ship* >::iterator i = residentShips.begin(); i != residentShips.end(); i++ ) {
			double shipX = location.getX() + 0.02 * cos( offset + counter * 2 * M_PI / residentShips.size() );
			double shipY = location.getY() + 0.02 * sin( offset + counter * 2 * M_PI / residentShips.size() );
			(*i)->setLocation( shipX, shipY );
			counter++;
		}
	}
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
Planet::createShip( const Uint32& time, Player* player ) {
	if( player->getPlayerType() != Player::NEUTRAL )
		if( owner == player ) {
    	player->addShips( this, 1 );
    	buildStartTime = time;
    	buildEndTime = time + shipCreationSpeed;
    	universe->actionQueue->scheduleAction( buildEndTime, new CreateShipAction( player, this ) );
		}
}

void
Planet::moveResidentsTo(Planet *destination, int fleetSelection) {
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
    (*i)->moveTo(timer.getTime() + rand() % 500, destination, universe->actionQueue);
  }
}

// ##### PLANETS #####

Planets::Planets() {
}

Planets::Planets( int numberOfPlanets, int numberOfMoons ) {

	addPlanets( numberOfPlanets );
	addMoons( numberOfMoons );
	
}

void
Planets::addPlanets( int numberOfPlanets ) {

	for( int i = 0; i < numberOfPlanets; i++ ) {
		push_back( Planet() );
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
			done = !motherPlanet->getMoon();
		} while( !done );
		Planet p;
		p.makeMoon( &(*motherPlanet) );
		push_back( p );
	}
	
}

void 
Planets::render( ) const {
	for( const_iterator i = begin(); i != end(); i++ )
		i->render( );
}

void 
Planets::renderOrbits( ) const {
	for( const_iterator i = begin(); i != end(); i++ )
		i->renderOrbit( );
}

Planet*
Planets::closestToCoordinate( const Coordinate& c ) {
	double closestDistance = 5000;
	Planet* closestPlanet = NULL;
	for( iterator i = begin(); i != end(); i++ ) {
		double distance = i->getLocation().distance( c );
		if( distance < closestDistance ) {
			closestDistance = distance;
			closestPlanet = &(*i);
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
		Coordinate location = i->getLocation();
		if( ( location.getX() > minX ) && ( location.getX() < maxX ) && ( location.getY() > minY ) && ( location.getY() < maxY ) )
			i->setSelected( true );
		else
			i->setSelected( false );
	}
	
}

void
Planets::sourceSelect( Selection *selection, Player *owner ) {

	double minX = selection->getMinX();
	double maxX = selection->getMaxX();
	double minY = selection->getMinY();
	double maxY = selection->getMaxY();
	
	for( iterator i = begin(); i != end(); i++ ) {
		Coordinate location = i->getLocation();
		if( ( location.getX() > minX ) && ( location.getX() < maxX )
		      && ( location.getY() > minY ) && ( location.getY() < maxY )
		      && i->getOwner() == owner )
			i->setSourceSelected( true );
		else
			i->setSourceSelected( false );
	}
	
}

void 
Planets::updateShipLocations() {
	for( iterator i = begin(); i != end(); i++ )
		i->updateShipLocations();
}

void
Planets::setUniverse( Universe* universe ) {
	for( iterator i = begin(); i != end(); i++ )
		i->setUniverse( universe );
}

Planet*
Planets::getRandomPlanet() {
	int number = rand() % size();
	iterator i = begin();
	for( int k = 0; k < number; k++ )
		i++;
	return &(*i);
}

Planet*
Planets::getRandomEnemyPlanet( Player* player ) {
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

CreateShipAction::CreateShipAction() :
Action::Action() {
	actionID = 1;
}

CreateShipAction::CreateShipAction( Player* player, Planet* planet ) {
	this->player = player;
	this->planet = planet;
	actionID = 1;
}

void
CreateShipAction::execute( const Uint32& time ) {
	planet->createShip( time, player );
}

Planet*
CreateShipAction::getPlanet() const {
	return planet;
}
