// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include <math.h>

#include "coordinate.h"

#include "settings.h"

Coordinate::Coordinate() {
	x = y = 0;
}

Coordinate::Coordinate( double x, double y ) {
	this->x = x;
	this->y = y;
}

void
Coordinate::setX( double x ) {
	this->x = x;
}

void
Coordinate::setY( double y ) {
	this->y = y;
}

void
Coordinate::setXMapped( int x ) {
	this->x = ( (double)x - Settings::getGameOffsetX() ) / Settings::getGameWidth();
}

void
Coordinate::setYMapped( int y ) {
	this->y = (double)y / Settings::getGameHeight();
}

double
Coordinate::getX() const {
	return x;
}

double
Coordinate::getY() const {
	return y;
}

int
Coordinate::getXMapped() const {
	return Settings::getGameOffsetX() + ( int )( Settings::getGameWidth() * x );
}

int
Coordinate::getYMapped() const {
	return ( int )( Settings::getGameHeight() * y );
}

double
Coordinate::distance( const Coordinate& c ) const {
	double dx = c.getX() - getX();
	double dy = c.getY() - getY();
	return sqrt( dx * dx + dy * dy );
}

