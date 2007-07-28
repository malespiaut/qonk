// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef PLANETS_H
#define PLANETS_H

#include <list>

#include "actions.h"
#include "coordinate.h"

class Ship;
class Universe;
class Player;
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
  
  Coordinate locationVector;
	
public:
	Planet(); // make a planet
  
	void makeMoon( Planet* );
  Coordinate getLocation() const;
	
	void addResident( Ship* ship, Uint32 time );
	void createShip( const Uint32& time);
	void removeResident( Ship* ship );
	
	void render(Uint32) const;
	void renderOrbit( ) const;
	void renderBuildProgress(Uint32) const;

  void update(Uint32);	
	void updateShipLocations(Uint32);
	
	double distance( Planet* planet );
	
	int numberOfResidentShips() { return residentShips.size(); }
	void setSourceSelected( bool selected );
	bool isSourceSelected() { return sourceSelected; }

	void setNearestPlanet(bool b);
	bool isNearestPlanet() { return nearestPlanet; }

  void moveResidentsTo(Uint32, Planet *, int);
	
	void setSelected( bool selected );
	void setUniverse( Universe* universe );
	bool getMoon() const { return isAMoon; }

  
  void setOwner(Player *);
  Player* getOwner() const { return owner; } 

};

class Planets : public std::list< Planet *> { 
public:
	Planets();
	Planets( int numberOfPlanets, int numberOfMoons );
  ~Planets();
	
	void addPlanets( int numberOfPlanets );
	void addMoons( int numberOfMoons );

	void render(Uint32) const;
	void renderOrbits( ) const;
	
	Planet* closestToCoordinate( const Coordinate& c );
	Planet* closestToCoordinate( const Coordinate& c, double treshold );
	
	void select( Selection selection );
	void sourceSelect( Selection *selection, Player *owner );
	
	void update(Uint32);
	
	void setUniverse( Universe* universe );
	
	Planet* getRandomPlanet();
	Planet* getRandomEnemyPlanet( Player* );
	Planet* getRandomFriendlyPlanet( Player* );
	Planet* getRandomNearbyPlanet( Planet* );
	
};

class CreateShipAction : public Action {
private:
	Planet *planet;
public:
	CreateShipAction(Planet *);
	void execute( const Uint32& time );
};

#endif
