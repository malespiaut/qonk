#include "game.h"
#include "gameoptions.h"

#include "planets.h"
#include "players.h"
#include "universe.h"
#include "messages.h"
#include "timer.h"
#include "settings.h"
#include "canvas.h"
#include "selection.h"

#include "extensions.h"

using namespace std;

Game::Game(GameOptions &go)
  : gameOptions(go), nextNumberOfPlanets(0), nextNumberOfComputerPlayers(0)
{
  int numberOfPlanets = gameOptions.getPlanets();
  int numberOfComputerPlayers = gameOptions.getPlayers();

	  state = PLAYING;
    quit = false;

     x = Settings::getScreenWidth()/2;
     y = Settings::getScreenHeight()/2;
     xp = xn = yp = yn = 0;

     universe = new Universe( numberOfPlanets );

     players = new Players( universe );
     universe->actionQueue->scheduleAction( 1000, new UpdatePlayersStatsAction( players) );

     Planets::iterator planetIterator = universe->planets->begin();
     humanPlayer = new HumanPlayer( universe, &(*planetIterator), 3, 0xffffff, 1);
     humanPlayer->setVisible(true);
     players->push_back(humanPlayer);

     planetIterator++;

     int hueCounter = rand() % 360;
        int counter = 0;
        do {
                Player *newPlayer;

                if( counter < numberOfComputerPlayers ) {
                        newPlayer = new ComputerPlayer( universe, &(*planetIterator), 3, HSVtoRGB( hueCounter, 0.9, 0.9 ), 2);

                        counter++;
                        hueCounter = ( hueCounter + 360 / numberOfComputerPlayers ) % 360;
                } else
                        if( !(&(*planetIterator))->getMoon() )
                                newPlayer = new NeutralPlayer( universe, &(*planetIterator), 1 + rand() % 3, 0x606060 );
                        else
                                newPlayer = new NeutralPlayer( universe, &(*planetIterator), 1 + rand() % 2, 0x606060 );
                players->push_back(newPlayer);

                planetIterator++;
        } while( planetIterator != universe->planets->end() );

        messages = new Messages();
        selection = new Selection();

  setEnemyVisibility(gameOptions.getEnemyVisibility());
}

Game::~Game()
{
  delete players;
  delete universe;
  delete messages;
  delete selection;
}

bool
Game::run(bool menuVisible)
{
  if (quit)
    return false;

  // TODO: Find a way to live without the next line.
  setPaused(menuVisible);

	if (timer.isPaused())
    return true;

  moveCursor(xp - xn, yp - yn);

  // update the universe
  universe->update();

  // let players update their states
  players->update(this);
		
	if( state == PLAYING )
  {
    if(checkLost()) {
      setEnemyVisibility(true);
      state = LOST;
      MSGlost(this);
    }

		if( checkWin()) {
      nextNumberOfPlanets = gameOptions.getPlanets() + 1;
      nextNumberOfComputerPlayers = gameOptions.getPlayers() + 1;
        
      if( nextNumberOfPlanets == 11 )
      {
        int d = nextNumberOfPlanets - nextNumberOfComputerPlayers;
        if( d != 1 )
          nextNumberOfPlanets = 10;
      }

			MSGwon(this, nextNumberOfPlanets, nextNumberOfComputerPlayers);
			state = WON;
		}
	}

  return true;
}

void
Game::handle(GameAction gameAction, int value)
{
  if (timer.isPaused())
    return;

  switch (gameAction)
  {
    case GA_NEXT_ROUND:
      if (!value) return;
      if(state==WON)
      {
        quit = true;
      }
      gameOptions.setPlanets(nextNumberOfPlanets);
      gameOptions.setPlayers(nextNumberOfComputerPlayers);

      break;
    case GA_RESTART_ROUND:
      if (!value) return;
      quit = true;
      break;
    case GA_SELECT_ALL:
      if (value)
        selectAllPlanets();
      break;
    case GA_TOGGLE_ENEMY_VISIBILITY:
      if (value)
      {
        gameOptions.setEnemyVisibility(!gameOptions.getEnemyVisibility());
        setEnemyVisibility(gameOptions.getEnemyVisibility());
      }
      break;
    case GA_SELECTION:
      if (value)
        startSelection(x, y);
      else
        endSelection(x, y);
      break;
    case GA_SELECT_NEAREST_PLANET:
      if (value)
        selectNearestPlanet(x, y);
      break;
    case GA_MOVE_TO_NEAREST_PLANET:
      if (value)
        moveToNearestPlanet(x, y);
      break;
    case GA_FLEET_STRENGTH_UP:
      if (value)
        fleetStrengthUp();
      break;
    case GA_FLEET_STRENGTH_DOWN:
      if (value)
        fleetStrengthDown();
      break;
    case GA_SET_FLEET_STRENGTH_SINGLE:
      if (value)
        setFleetStrength(1);
      break;
    case GA_CURSOR_UP:
        yn = value;
        break;
    case GA_CURSOR_DOWN:
        yp = value;
        break;
    case GA_CURSOR_LEFT:
        xn = value;
        break;
    case GA_CURSOR_RIGHT:
        xp = value;
        break;
    default:
      if (value 
          && gameAction >= GA_SET_FLEET_STRENGTH_10
          && gameAction <= GA_SET_FLEET_STRENGTH_100)
      {
        setFleetStrength(10 * (1 + gameAction - GA_SET_FLEET_STRENGTH_10));
      }
  }
}

void
Game::moveCursor(int xrel, int yrel)
{
  x += xrel;
  y += yrel;

  x = max(0, min(x, Settings::getScreenWidth()));
  y = max(0, min(y, Settings::getScreenHeight()));
  
  if (selection->isSelecting())
    selection->update(x, y);
  else
    universe->highlightNearestPlanet(x, y);
}

bool
Game::checkWin()
{
        bool allPlayersDead = true;
        for( Players::iterator i = players->begin(); i != players->end(); i++ ) {
                if( (*i)->getTeam() == 2 )
                
                if( ((ComputerPlayer*)(*i))->getPoints() > 0 )
                        allPlayersDead = false;
        }

        return allPlayersDead;
}

bool
Game::checkLost()
{
  return humanPlayer->getPoints() == 0;
}

void
Game::fleetStrengthUp()
{
  int str = humanPlayer->getFleetStrength();
  if (str == 1)
    str = 10;
  else if (str == 100)
    str = 1;
  else
    str += 10;

  setFleetStrength(str);
}

void
Game::fleetStrengthDown()
{
  int str = humanPlayer->getFleetStrength();
  if (str == 1)
    str = 100;
  else if (str == 10)
    str = 1;
  else
    str -= 10;

  setFleetStrength(str);
}

void
Game::setFleetStrength(int str)
{
  humanPlayer->setFleetStrength(str);
/*
  stringstream s;
  if (str == 1)
    s << "Setting to single ship mode.";
  else
    {
      s << "Setting fleet strength to " << str << "%";
    }

  messages->addMessage( timer.getTime(), Message(s.str(), 20000, 0x808080));
  */  
  
  //Begin Jacobsen
  stringstream s;
  if (str == 1)
	  s << "Single ship mode";
  else
  {
	  s << "Fleet strength " << str << "%";
  }

  messages->setFleetStrengthMessage( Message(s.str(), 0x808080) );
  //End Jacobsen
}

void
Game::setPaused(bool p)
{
  if (p ^ timer.isPaused())
    timer.pause();

}

void
Game::setEnemyVisibility(bool visible)
{
  int humanTeam = humanPlayer->getTeam();

  for( Players::iterator i = players->begin(); i != players->end(); i++ )
  {
    Player *p = *i;
    if (p->getTeam() != humanTeam)
      p->setVisible(visible);
  }

  stringstream s;
  s << "Made enemies " << (visible ? "visible" : "invisible");

  messages->addMessage(timer.getTime(), Message(s.str(), 20000, 0x808080));
}

void
Game::selectAllPlanets()
{
  humanPlayer->selectAllPlanets();
}

void
Game::selectNearestPlanet(int x, int y)
{
  humanPlayer->selectNearestPlanet(x, y);
}

void
Game::moveToNearestPlanet(int x, int y)
{
  humanPlayer->moveToNearestPlanet(x, y);
}

void
Game::startSelection(int x, int y)
{
  selection->start(x, y);
}

void
Game::updateSelection(int x, int y)
{
  selection->update(x, y);
}

void
Game::endSelection(int x, int y)
{
  selection->end();

  if (selection->isEmpty())
    humanPlayer->selectPlanetAt(x, y);
  else
    universe->planets->sourceSelect(selection, humanPlayer);
}

void
Game::addMessage(int offset, Message msg)
{
  messages->addMessage( timer.getTime() + offset, msg);
}

void
Game::render()
{
        universe->renderBackground();
        players->render();

        humanPlayer->render();
        selection->render();

        universe->renderForeground();

        messages->render();

        Canvas::drawCursor(x, y);
}
