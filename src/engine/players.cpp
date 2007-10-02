// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include <map>

#include "players.h"
#include "planets.h"
#include "universe.h"
#include "ships.h"
#include "timer.h"
#include "selection.h"
#include "messages.h"
#include "canvas.h"
#include "game.h"

#include "extensions.h"

// ##### PLAYER #####
using namespace std;

Player::Player( Universe* universe, Uint32 color, int t ) : visible(false), team(t)
{
  ships = new Ships();
  playerState = ALIVE;
  this->universe = universe;
  setColor( color );
}

void
Player::addShip(Uint32 time, Planet * const planet)
{
  ships->push_back(new Ship(this, planet, time ));
}

void
Player::removeDeadShips() {
  ships->removeDeadShips();
}

void
Player::addPlanet(Planet *newPlanet)
{
  occupiedPlanets.push_back(newPlanet);
}

void
Player::removePlanet(Planet *lostPlanet)
{
  occupiedPlanets.remove(lostPlanet);
}

void
Player::updateStats( double maximumPoints ) {
  stats.push_back( (int)( getPoints() / maximumPoints * Canvas::getHeight()  + 0.5 ) );
  if( stats.size() == 1 ) {
    for( int i = 0; i < 127; i++ )
      stats.push_back( (int)( getPoints() / maximumPoints * Canvas::getHeight()  + 0.5 ) );
  }
  while( stats.size() > 128 ) {
    stats.erase( stats.begin() );
  }
}

void
Player::render()
{
  if (visible)
    ships->render(color);
}

bool
Player::renderStats( int height ) {
  int counter = 4;
  for( Planets::iterator i = universe->planets->begin(); i != universe->planets->end(); i++ ) {
    if( (*i)->getOwner() == this ) {
      int size = 3;
      if( (*i)->getMoon() ) size = 2;
      Canvas::drawPlanetMapped(counter, height + 5, size, color );
      counter+= size * 2 + 3;
    }
  }
//  if( getPoints() == 0 ) {
//    boxRGBA( screen, counter - 1, height + 2, counter + 1, height + 8, getRed( color ), getGreen( color ), getBlue( color ), 255 );
//    boxRGBA( screen, counter - 3, height + 4, counter + 3, height + 6, getRed( color ), getGreen( color ), getBlue( color ), 255 );
//    counter += 6;
//  }
  return true;
//  return ( counter != 4 );
}

void
Player::renderStatsLog() {
  if( playerType == HUMAN )
  {
    int size = stats.size();
    for( int i = 1; i < size; i++ ) {
      Canvas::drawPlayerStat(size, i, stats[i-1], stats[i], 
                             getRed( color ), getGreen( color ), getBlue( color ));
    }
  }
}

double
Player::getPoints() {
  double result = 0;
  for( Planets::iterator i = universe->planets->begin(); i != universe->planets->end(); i++ ) {
    if( (*i)->getOwner() == this ) {
      if( (*i)->getMoon() ) { 
        result += 0.999;
      } else {
        result += 2;
      }
    }
  }
  result += ships->size() / 3.0;
  return result;
}

Player::PlayerTypes
Player::getPlayerType() {
  return playerType;
}

void
Player::setColor( const Uint32& color ) {
  this->color = color;
}

Uint32
Player::getColor() {
  return color;
}

// ##### HUMANPLAYER #####

HumanPlayer::HumanPlayer( Universe* universe, Uint32 color, int team ) :
Player( universe, color, team ) {
  playerType = Player::HUMAN;
  fleetStrength = 50;
}

void
HumanPlayer::selectNearestPlanet(Coordinate &c)
{
  Planet* closestPlanet = universe->planets->closestToCoordinate( c, 1 );
	  
  if (closestPlanet && closestPlanet->getOwner() == this) {
  for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++)
    (*i)->setSourceSelected(false);
	    
    closestPlanet->setSourceSelected(true);
  }
  else
  {
    for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++)
      (*i)->setSourceSelected(false);
  }

}

void
HumanPlayer::selectPlanetAt(Coordinate &c)
{
  Planet* closestPlanet = universe->planets->closestToCoordinate( c, 0.01 );
	  
  if (closestPlanet && closestPlanet->getOwner() == this) {
    for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++)
      (*i)->setSourceSelected(false);
	    
    closestPlanet->setSourceSelected(true);
  }
  else
  {
    for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++)
      (*i)->setSourceSelected(false);
  }

}

void
HumanPlayer::moveToNearestPlanet(Uint32 time, Coordinate &c)
{
  Planet* closestPlanet = universe->planets->closestToCoordinate( c, 1 );
	  
  if( closestPlanet != NULL ) {
    for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++) {
      if ((*i)->isSourceSelected())
        (*i)->moveResidentsTo(time, closestPlanet, fleetStrength);
    }
  }
}

void
HumanPlayer::update(Game *game) {

  if( playerState == ALIVE ) {
    if( getPoints() == 0 ) {
      playerState = DEAD;
      return;
    }
  } else
    return;
    
  removeDeadShips();

  // TODO: Unify this with the other player types.  
  ships->update(game->getTime());
	
}

void
HumanPlayer::setFleetStrength(int str) {
  if (fleetStrength != str) {
   fleetStrength = str;
  }
}

void
HumanPlayer::selectAllPlanets() {
  for (list <Planet *>::iterator i = occupiedPlanets.begin(); i != occupiedPlanets.end(); i++)
    (*i)->setSourceSelected(true);
}

// ##### COMPUTERPLAYER #####

ComputerPlayer::ComputerPlayer( Universe* universe, Uint32 color, int team ) :
Player( universe, color, team ) {
  playerType = Player::COMPUTER;
  
  // give the computer player some values for determining strategic actions
  
  proportionResidents = 0.3 + frand( 0.4 );
  moonPriority = 0.5 + frand( 0.7 );
  enemyPriority = 0.05 + frand( 0.45 );
  preferredPriority = 0.2 + frand( 0.5 );
  neutralPriority = 0.2 + frand( 0.6 );
  weakerEnemyPriority = 0.2 + frand( 1.6 );
  strongerEnemyPriority = 0.2 + frand( 1.6 );
  
  universe->actionQueue->scheduleAction( rand() % 10000, new ComputerPlayerAction( this ) );
}

void 
ComputerPlayer::printStats() {
  cout << "proportionResidents:" << proportionResidents << " moonPriority:" << moonPriority << " enemyPriority:" << enemyPriority << " preferredPriority:" << preferredPriority << " neutralPriority:" << neutralPriority << endl;
}

void
ComputerPlayer::update(Game *game) {
  if( playerState == ALIVE ) {
    if( getPoints() == 0 ) {
      game->addMessage(0, Message(game->getTime(), "AI player is dead", color));
      playerState = DEAD;
      return;
    }
  } else
    return;
  removeDeadShips();
  
  // TODO: Unify this with the other player types.  
  ships->update(game->getTime());
  
}



bool
ComputerPlayer::isAPreferredPlanet( Planet* planet ) {
  for( list< Planet* >::iterator i = preferredPlanets.begin(); i != preferredPlanets.end(); i++ )
    if( (*i) == planet )
      return true;
  return false;
}

Planet*
ComputerPlayer::getRandomNearbyPlanet( Planet* planet ) {

	Planet* result;
	do {
		result = universe->planets->getRandomPlanet();
	} while( result == planet );
	double distance = planet->distance( result );
	
	for( int i = 0; i < universe->planets->size() * 2; i++ ) {
		Planet* otherPlanet;
		do {
			otherPlanet = universe->planets->getRandomPlanet();
		} while( otherPlanet == planet );
		double otherDistance = planet->distance( otherPlanet );
		
		// which planets do we prefer?
		
		if( otherPlanet->getOwner() != planet->getOwner() ) {
			otherDistance *= enemyPriority;
			if( isAPreferredPlanet( otherPlanet ) ) {
			  otherDistance *= preferredPriority;
      }
      if( otherPlanet->getOwner()->getPlayerType() != Player::NEUTRAL ) {
        if( otherPlanet->getOwner()->getPoints() < getPoints() )
          otherDistance *= weakerEnemyPriority;
        else
          otherDistance *= strongerEnemyPriority;
      }
    }
		if( otherPlanet->getOwner()->getPlayerType() == Player::NEUTRAL )
			otherDistance *= neutralPriority;
		if( !otherPlanet->getMoon() )
			otherDistance *= moonPriority;
		
		if( otherDistance < distance ) {
			result = otherPlanet;
			distance = otherDistance;
		}
	}
	
	return result;

}



void
ComputerPlayer::action( const Uint32& time ) {

  // The AI of the computer player ... it's not much, but it does some
  // intelligent things
  // 
  // * compute the number of ships we want to launch
  // * divide this into a set of squadrons
  // * each squadron (nearby resident ships) launches to a nearby planet
  // * schedule our next activity
  
  // how many (resident) ships do i have, and how many do I want to launch
  // according to my strategy?
  
  int numberOfShips = ships->size();
  int numberOfResidents = ships->countResidents();
  int numberOfLaunches = numberOfResidents - (int)( ships->size() * proportionResidents ) - 1;
  
  if( numberOfLaunches > 0 ) {
  
    Ship* ship;
    Planet* sourcePlanet;
    Planet* destinationPlanet;
    
    int squadronSize = rand() % numberOfLaunches + 1;
    if( squadronSize > 10 ) {
      squadronSize = 10;
    }
  
    ship = ships->getRandomResidentShip();

    sourcePlanet = ship->getPlanet(); 
    destinationPlanet = getRandomNearbyPlanet( sourcePlanet );
    if( ( destinationPlanet->getOwner() == this ) && ( destinationPlanet->numberOfResidentShips() > sourcePlanet->numberOfResidentShips() ) )
      destinationPlanet = sourcePlanet;
    
    if( !isAPreferredPlanet( destinationPlanet ) )
    if( destinationPlanet->getOwner() != this )
      preferredPlanets.push_back( destinationPlanet );
    if( preferredPlanets.size() > 3 )
      preferredPlanets.erase( preferredPlanets.begin() );
  
    int launchedShips = 0;
  
    for( int i = 0; i < squadronSize; i++ ) {
    
      Ship* nextShip = ships->getRandomNearbyResidentShip( ship );
    
      if( nextShip != NULL ) {
        if( nextShip->distance( ship ) < 0.25 ) {
          nextShip->moveTo( time + rand() % 500, destinationPlanet, universe->actionQueue );
          launchedShips++;
          ship = nextShip;
        }
      }
      
    }
    
    int wait = (int)( ( ( 1 + launchedShips ) / (double)numberOfLaunches ) * ( 2000 + rand() % 1000 ) );
    universe->actionQueue->scheduleAction( time + wait, new ComputerPlayerAction( this ) );
    return;

  }

  int wait = 3000 + rand() % 1500;
  universe->actionQueue->scheduleAction( time + 4000 + rand() % 2000, new ComputerPlayerAction( this ) );
  
}

// ##### NEUTRALPLAYER #####

NeutralPlayer::NeutralPlayer( Universe* universe, Uint32 color ) :
Player( universe, color, 0 ) {
  playerType = Player::NEUTRAL;
}

void
NeutralPlayer::update(Game *game) {
  removeDeadShips();
  
   // TODO: Unify this with the other player types.  
  ships->update(game->getTime());
  
}

// ##### PLAYERS #####

Players::Players( Universe* universe ) {
  this->universe = universe;
  bestPlayer = NULL;
}

void
Players::render() {
  for( iterator i = begin(); i != end(); i++ ) {
    (*i)->render();
  }
  renderStats();
}

double
Players::getSumOfScores() {
  double sum = 0;
  for( iterator i = begin(); i != end(); i++ ) {
    if( (*i)->getPlayerType() != Player::NEUTRAL )
      sum += (*i)->getPoints();
  }
  return sum;
}

void
Players::updateStats( int time ) {
  double sumOfScores = getSumOfScores();
  for( iterator i = begin(); i != end(); i++ ) {
    (*i)->updateStats( sumOfScores );
  }
  universe->actionQueue->scheduleAction( time + 1000, new UpdatePlayersStatsAction( this ) );
}

void
Players::renderStats() {


  multimap< double, Player* > playerSorter;
  for( iterator i = begin(); i != end(); i++ ) {
    playerSorter.insert( pair< double, Player* >( (*i)->getPoints(), *i ) );
    (*i)->renderStatsLog();
  }
  
/* TODO: Reimplement this with a scoreboard approach or something else
  Player* currentBestPlayer = playerSorter.rbegin()->second;
  double maxPoints = currentBestPlayer->getPoints();
  if( currentBestPlayer != bestPlayer ) {
    bestPlayer = currentBestPlayer;
    if( bestPlayer->getPlayerType() == Player::COMPUTER )
      universe->messages->addMessage( timer.getTime(), Message( "AI player takes the lead", 10000, getRed( bestPlayer->getColor() ), getGreen( bestPlayer->getColor() ), getBlue( bestPlayer->getColor() ) ) );
    else if( bestPlayer->getPlayerType() == Player::HUMAN )
      universe->messages->addMessage( timer.getTime(), Message( "You take the lead", 10000, getRed( bestPlayer->getColor() ), getGreen( bestPlayer->getColor() ), getBlue( bestPlayer->getColor() ) ) );
  }
*/

  int height = 0;
  for( multimap< double, Player* >::reverse_iterator i = playerSorter.rbegin(); i != playerSorter.rend(); i++ ) {
    Player* p = i->second;
    if( p->getPlayerType() != Player::NEUTRAL ) {
      if( p->renderStats(height ) )
        height += 10;
    }
  }
        
}

void
Players::update(Game *game) {
  for( iterator i = begin(); i != end(); i++ ) {
    (*i)->update(game);
  }
}

// ##### COMPUTERPLAYERACTION #####

ComputerPlayerAction::ComputerPlayerAction() {
}

ComputerPlayerAction::ComputerPlayerAction( ComputerPlayer* player ) {
  this->player = player;
}

void
ComputerPlayerAction::execute( const Uint32& time ) {
  player->action( time );
}

// ##### UPDATEPLAYERSSTATS #####

UpdatePlayersStatsAction::UpdatePlayersStatsAction() {
}

UpdatePlayersStatsAction::UpdatePlayersStatsAction( Players* players ) {
  this->players = players;
}

void
UpdatePlayersStatsAction::execute( const Uint32& time ) {
  players->updateStats( time );
}
