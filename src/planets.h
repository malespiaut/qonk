// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef PLANETS_H
#define PLANETS_H

#include <list>

#include "actions.h"

class Ship;
class Universe;
class Player;
class Coordinate;
class Selection;

class Planet {
private:
	Universe* universe;
	Player* owner;
	int rotationSpeed, rotationOffset;
	double rotationDistance;
	int shipCreationSpeed;
	int size;
	bool isAMoon, selected, sourceSelected, nearestPlanet;
	Planet* mother;
	std::list< Ship* > residentShips;
	Uint32 buildStartTime, buildEndTime;
	
public:
	Planet(); // make a planet
	void makeMoon( Planet* );
	Coordinate getVector( Uint32 time ) const;
	Coordinate getLocation() const;
	Coordinate getLocation( Uint32 time ) const;
	
	void addResident( Ship* ship, Uint32 time );
	void createShip( const Uint32& time, Player* player );
	void removeResident( Ship* ship );
	
	void render( ) const;
	void renderOrbit( ) const;
	void renderBuildProgress( ) const;
	
	void updateShipLocations();
	
	double distance( Planet* planet );
	
	int numberOfResidentShips() { return residentShips.size(); }
	void setSourceSelected( bool selected );
	bool isSourceSelected() { return sourceSelected; }

	void setNearestPlanet(bool b);
	bool isNearestPlanet() { return nearestPlanet; }
	
	void setSelected( bool selected );
	void setUniverse( Universe* universe );
	bool getMoon() const { return isAMoon; }
	Player* getOwner() { return owner; }
	
	void moveResidentsTo(Planet *, int);
private:
	void setOwner(Player *);
};

class Planets : public std::list< Planet > { 
public:
	Planets();
	Planets( int numberOfPlanets, int numberOfMoons );
	
	void addPlanets( int numberOfPlanets );
	void addMoons( int numberOfMoons );

	void render( ) const;
	void renderOrbits( ) const;
	
	Planet* closestToCoordinate( const Coordinate& c );
	Planet* closestToCoordinate( const Coordinate& c, double treshold );
	
	void select( Selection selection );
	void sourceSelect( Selection *selection, Player *owner );
	
	void updateShipLocations();
	
	void setUniverse( Universe* universe );
	
	Planet* getRandomPlanet();
	Planet* getRandomEnemyPlanet( Player* );
	Planet* getRandomFriendlyPlanet( Player* );
	Planet* getRandomNearbyPlanet( Planet* );
	
};

class CreateShipAction : public Action {
private:
	Player* player;
	Planet* planet;
public:
	CreateShipAction();
	CreateShipAction( Player* player, Planet* planet );
	void execute( const Uint32& time );
	Planet* getPlanet() const;
};

#endif
