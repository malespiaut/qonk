// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef PLAYERS_H
#define PLAYERS_H

#include <vector>
#include <list>

#include "actions.h"

class Ships;
class Universe;
class Planet;
class Selection;
class Game;

class Player {
public:
  enum PlayerTypes { HUMAN, COMPUTER, NEUTRAL };
  enum PlayerStates { ALIVE, DEAD };
protected:
  Universe* universe;
  Ships* ships;
  PlayerTypes playerType;
  PlayerStates playerState;
  Uint32 color;
  std::vector< int > stats;
  std::list< Planet * > occupiedPlanets;
  bool visible;
  int team;
  
public:
  Player(Universe *, Uint32, int);
  virtual ~Player() {}
  
  void addShip(Uint32, Planet * const);
  void removeDeadShips();
  
  void render();

  void setVisible(bool vis) { visible = vis; };
  void updateStats( double maximumPoints );
  bool renderStats( int height );
  void renderStatsLog( );
  virtual void update(Game *game) = 0;
  
  PlayerTypes getPlayerType();
  
  void setColor( const Uint32& );
  Uint32 getColor();
  Universe* getUniverse() { return universe; }
  double getPoints();
  
  void addPlanet(Planet *);
  void removePlanet(Planet *);

  int getTeam() const { return team; }

};

class HumanPlayer : public Player {
private:
  int fleetStrength;
public:
  HumanPlayer( Universe* universe, Uint32 color, int team );
  
  void update(Game *game);
  
  void selectAllPlanets();

  void setFleetStrength(int);

  int getFleetStrength() const { return fleetStrength; }

  void selectPlanetAt(int, int);

  void selectNearestPlanet(int, int);

  void moveToNearestPlanet(Uint32, int, int);
};

class ComputerPlayer : public Player {
private:
  // strategy related
  
  double proportionResidents; // proportion of all ships to reside on planets
  double moonPriority, enemyPriority, preferredPriority, neutralPriority, weakerEnemyPriority, strongerEnemyPriority; 
  
  std::list< Planet* > preferredPlanets;

public:
  ComputerPlayer( Universe* universe, Uint32 color, int team );
  
  void update(Game *);
  
  void action( const Uint32& time );
  
  bool isAPreferredPlanet( Planet* );
  Planet* getRandomNearbyPlanet( Planet* );
  void printStats();
};

class NeutralPlayer : public Player {
public:
  NeutralPlayer( Universe* universe, Uint32 color);
  
  void update(Game *);
  
};

class Players : public std::list< Player* > {
protected:
  Universe* universe;
  Player* bestPlayer;
public:
  Players( Universe* universe );

  void render( );
  double getSumOfScores();
  void updateStats( int time );
  void renderStats( );
  
  void update(Game *);
};

class ComputerPlayerAction : public Action {
private:
  ComputerPlayer* player;
public:
  ComputerPlayerAction();
  ComputerPlayerAction( ComputerPlayer* player );
  void execute( const Uint32& time );
};

class UpdatePlayersStatsAction : public Action {
private:
  Players* players;
public:
  UpdatePlayersStatsAction();
  UpdatePlayersStatsAction( Players* players );
  void execute( const Uint32& time );
};

#endif
