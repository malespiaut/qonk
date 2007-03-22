// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "ships.h"
#include "extensions.h"
#include <SDL_gfxPrimitives.h>
#include <iostream>

/* ##### SHIP ##### */

Ship::Ship( Player* owner, Planet* planet ) {
	this->owner = owner;
	this->planet = planet;
	planet->addResident( this, timer.getTime() );
	planet->updateShipLocations();
	selected = false;
	shipState = RESIDENT;
	speed = 0.003 + frand( 0.002 );
}

void
Ship::die() {
	shipState = DEAD;
}

void
Ship::moveTo( Uint32 time, Planet* destinationPlanet, ActionQueue* actionQueue ) {
	if(shipState == RESIDENT) {
	
		if( planet != destinationPlanet ) {
		
			// we're going to start moving
		
			shipState = MOVING;
			planet->removeResident( this );
			setLocation( planet->getLocation() );
			selected = false;
			
		} else {
		
			// we are already at the planet where we are supposed to go, so no need to move
		
			selected = false;
			return; 
			
		}
		
	} else {
	
		// we're already moving
	
		setLocation( toLocation );
		
	}
	
	// schedule our movement for the next 100 milliseconds
	
	fromTime = time;
	toTime = time + 100;
	fromLocation = location;
	Coordinate destinationLocation = destinationPlanet->getLocation( time + 100 );
	double distance = fromLocation.distance( destinationLocation );
	if( distance < speed ) {
	
		// we are close enough to arrive at our destination planet
		
		shipState = RESIDENT;
		planet = destinationPlanet;
		destinationPlanet->addResident( this, time );
		
	} else {
	
		// we are on the move, schedule the next movement step within 100 milliseconds
	
		double dx = destinationLocation.getX() - fromLocation.getX();
		double dy = destinationLocation.getY() - fromLocation.getY();
		dx = speed * dx / distance;
		dy = speed * dy / distance;
		toLocation.setX( fromLocation.getX() + dx );
		toLocation.setY( fromLocation.getY() + dy );
		ShipMovementAction* shipMovementAction = new ShipMovementAction( this, destinationPlanet );
		actionQueue->scheduleAction( toTime, shipMovementAction );
	}
}

void
Ship::render( SDL_Surface* screen, Uint32 color ) const {
	Coordinate loc = getLocation();
	int locx = loc.getXMapped();
	int locy = loc.getYMapped();
	int red = getRed( color );
	int green = getGreen( color );
	int blue = getBlue( color );
	if( shipState == RESIDENT ) {
		Coordinate planetLocation = planet->getLocation();
		aalineRGBA( screen, locx, locy, planetLocation.getXMapped(), planetLocation.getYMapped(), red, green, blue, 64 );
	}
//	filledCircleRGBA( screen, locx, locy, 1, getRed( color ), getGreen( color ), getBlue( color ), 255 );
	int direction = getDirection();
	switch (direction) {
		case 0 :
			//Cross
			pixelRGBA( screen, locx, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx + 1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx - 1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy + 1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy - 1, red, green, blue, 255 );
			break;
		case 1 :
			pixelRGBA( screen, locx+2, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy, red, green, blue, 255 );
			break;
		case 2 :
			pixelRGBA( screen, locx+2, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy-2, red, green, blue, 255 );
			break;
		case 3 :
			pixelRGBA( screen, locx+2, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy, red, green, blue, 255 );
			break;
		case 4 :
			// Ship pointing down
			pixelRGBA( screen, locx, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy-2, red, green, blue, 255 );
			break;
		case 5 :
			pixelRGBA( screen, locx-2, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy, red, green, blue, 255 );
			break;
		case 6 :
			pixelRGBA( screen, locx-2, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy-2, red, green, blue, 255 );
			break;
		case 7 :
			pixelRGBA( screen, locx-2, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy, red, green, blue, 255 );
			break;
		case 8 :	
			// Ship pointing up
			pixelRGBA( screen, locx, locy-2, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy-1, red, green, blue, 255 );
			pixelRGBA( screen, locx+1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx-1, locy, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy+1, red, green, blue, 255 );
			pixelRGBA( screen, locx+2, locy+2, red, green, blue, 255 );
			pixelRGBA( screen, locx-2, locy+2, red, green, blue, 255 );
		break;
	}
}

void
Ship::renderSelection( SDL_Surface* screen ) const {
	Coordinate loc = getLocation();
	if( selected )
		aacircleRGBA( screen, loc.getXMapped(), loc.getYMapped(), 3, 255, 192, 0, 255 );
//	aacircleRGBA( screen, loc.getXMapped(), loc.getYMapped(), 2, 0, 0, 0, 255 );
}

double
Ship::distance( Ship* ship ) {
	return ship->getLocation().distance( getLocation() );
}

void
Ship::setLocation( double x, double y ) {
	location.setX( x );
	location.setY( y );
}

void
Ship::setLocation( const Coordinate& c ) {
	location = c;
}

Coordinate
Ship::getLocation() const {
	if( shipState == RESIDENT )
		return location;
	else {
		Coordinate result;
		result.setX( fromLocation.getX() + ( toLocation.getX() - fromLocation.getX() ) * ( timer.getTime() - fromTime ) / ( toTime - fromTime ) );
		result.setY( fromLocation.getY() + ( toLocation.getY() - fromLocation.getY() ) * ( timer.getTime() - fromTime ) / ( toTime - fromTime ) );
		return result;
	}
}

Ship::ShipStates
Ship::getShipState() const {
	return shipState;
}

void
Ship::setSelected( bool selected ) {
	this->selected = selected;
}

bool
Ship::getSelected() const {
	return selected;
}

double
Ship::getSpeed() const {
	return speed;
}

Player* 
Ship::getOwner() const {
	return owner;
}

Planet*
Ship::getPlanet() const {
	return planet;
}

int Ship::getDirection() const {
	if(shipState == RESIDENT){
		return 0;
	} else if(shipState == MOVING) {
		double dx = toLocation.getX() - location.getX();
		double dy = -(toLocation.getY() - location.getY());
		double dydx = dy/dx;
		if (dx > 0.0){
			if (dydx > 2.4142) {
				return 8;
			} else if (dydx < 2.4142 && dydx > 0.4142){
				return 1;
			} else if (dydx > -2.4142 && dydx < -0.4142){
				return 3;
			} else if(dydx < -2.4142){
				return 4;
			} else {
				return 2;
			}
		} else {
			if (dydx > 2.4142) {
				return 4;
			} else if (dydx < 2.4142 && dydx > 0.4142){
				return 5;
			} else if (dydx > -2.4142 && dydx < -0.4142){
				return 7;
			} else if(dydx < -2.4142){
				return 8;
			} else {
				return 6;
			}
		}
	}
	
}


/* ##### SHIPS ##### */

void
Ships::removeDeadShips() {
	remove_if( DeadShipRemovalFunctor() );
}

void
Ships::moveTo( Planet* destinationPlanet, ActionQueue* actionQueue ) {
	for( iterator i = begin(); i != end(); i++ ) {
		if( (*i)->getSelected() )
			(*i)->moveTo( timer.getTime() + rand() % 500, destinationPlanet, actionQueue );
	}
}

int
Ships::numberSelectedShips() {
	int counter = 0;
	for( iterator i = begin(); i != end(); i++ ) {
		if( (*i)->getSelected() )
			counter++;
	}
	return counter;
}

void
Ships::render( SDL_Surface* screen, Uint32 color ) const {
	for( const_iterator i = begin(); i != end(); i++ ) {
	 	(*i)->renderSelection( screen );
	}
	for( const_iterator i = begin(); i != end(); i++ ) {
		(*i)->render( screen, color );
	}
}

void
Ships::select( Selection* selection ) {

	double minX = selection->getMinX();
	double maxX = selection->getMaxX();
	double minY = selection->getMinY();
	double maxY = selection->getMaxY();
	
	for( iterator i = begin(); i != end(); i++ ) {
		Coordinate location = (*i)->getLocation();
		if( (*i)->getShipState() == Ship::RESIDENT ) // you can only select ships when they reside on a planet for now
			if( ( location.getX() > minX ) && ( location.getX() < maxX ) && ( location.getY() > minY ) && ( location.getY() < maxY ) )
				(*i)->setSelected( true );
			else
				(*i)->setSelected( false );
	}
	
}

void
Ships::selectAll() {

	for( iterator i = begin(); i != end(); i++ ) {
		if( (*i)->getShipState() == Ship::RESIDENT ) // you can only select ships when they reside on a planet for now
			(*i)->setSelected( true );
	}
	
}

int
Ships::countResidents() {
	int counter = 0;
	for( const_iterator i = begin(); i != end(); i++ ) {
		if( (*i)->getShipState() == Ship::RESIDENT )
			counter++;
	}
	return counter;
}

Ship*
Ships::getRandomShip() {
	int number = rand() % size();
	iterator i = begin();
	for( int k = 0; k < number; k++ )
		i++;
	return (*i);
}

Ship*
Ships::getRandomResidentShip() {
	if( countResidents() > 0 ) {
		Ship* result;
		bool done = false;
		do {
			result = getRandomShip();
			if( result->getShipState() == Ship::RESIDENT )
				done = true;
		} while( !done );
		return result;
	} else {
		return NULL;
	}
}

Ship*
Ships::getRandomNearbyResidentShip( Ship* ship ) {

	if( size() == 1 )
		return getRandomResidentShip();

	Ship* result;
	do {
		result = getRandomResidentShip();
	} while( result == ship );
	double distance = ship->distance( result );
	
	for( int i = 0; i < 10; i++ ) {

		Ship* otherShip;
		do {
			otherShip = getRandomResidentShip();
		} while( otherShip == ship );
		double otherDistance = ship->distance( otherShip );
		
		if( otherDistance < distance ) {
			result = otherShip;
			distance = otherDistance;
		}
		
	}
	
	return result;
}

Ship*
Ships::getNearestResidentShip( Planet* c ) {

	Ship* result = NULL;
	double max = 1000;
	
	Coordinate location = c->getLocation();
	
	for( iterator i = begin(); i != end(); i++ ) {
		if( (*i)->getShipState() == Ship::RESIDENT )
		if( (*i)->getPlanet() != c )
		if( location.distance( (*i)->getLocation() ) < max ) {
			result = (*i);
			max = location.distance( (*i)->getLocation() );
		}
	}

	return result;
}

/* ##### SHIPMOVEMENTACTION ##### */

ShipMovementAction::ShipMovementAction() {
}

ShipMovementAction::ShipMovementAction( Ship* ship, Planet* planet ) {
	this->ship = ship;
	this->destination = planet;
}

void
ShipMovementAction::execute( const Uint32& time ) {
	if( ship )
		ship->moveTo( time, destination, getActionQueue() );
}
