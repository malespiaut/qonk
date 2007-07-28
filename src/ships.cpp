// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include <iostream>
#include "math.h"

#include "coordinate.h"
#include "planets.h"
#include "timer.h"
#include "extensions.h"
#include "selection.h"
#include "canvas.h"

#include "ships.h"

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
Ship::render( Uint32 color ) const {
	Coordinate loc = getLocation();
	if( shipState == RESIDENT ) {
		Coordinate planetLocation = planet->getLocation();
		Canvas::drawResidentShip(loc, planetLocation, color);
	} else {
		Canvas::drawFlyingShip(loc, getDirection(), color);
	}
}

void
Ship::renderSelection() const {
	Coordinate loc = getLocation();
	if( selected )
	  Canvas::drawSelection(loc);
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

double
Ship::getDirection() const {
	if(shipState == RESIDENT){
		return 0;
	} else if( shipState == MOVING ) {
		double dx = toLocation.getX() - location.getX();
		double dy = toLocation.getY() - location.getY();
		double l = sqrt( dx*dx + dy*dy ); // length of vector
		double angle = acos( dx / l );
		if( dy < 0 ) angle = -angle;
		return angle;
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
Ships::render( Uint32 color ) const {
	for( const_iterator i = begin(); i != end(); i++ ) {
	 	(*i)->renderSelection();
	}
	for( const_iterator i = begin(); i != end(); i++ ) {
		(*i)->render(color );
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
