#include "game.h"

#include "timer.h"
#include "messages.h"
#include "selection.h"

#include "planets.h"
#include "players.h"
#include "universe.h"

#include "canvas.h"

#include "extensions.h"

/* TODO: Fix me */
#include "../gameoptions.h"

using namespace std;

Game::Game(GameOptions &go)
  : gameOptions(go), nextNumberOfPlanets(0), nextNumberOfComputerPlayers(0), timer(new Timer()),
    players(0), universe(0), messages(0), selection(0), cursor(0)
{
  int numberOfPlanets = gameOptions.getPlanets();
  int numberOfComputerPlayers = gameOptions.getPlayers();

  state = PLAYING;
  quit = false;
  timer->start();

  // Initialize selection
  cursor = new Coordinate();
  cursor->setXMapped(Canvas::getWidth()/2);
  cursor->setYMapped(Canvas::getHeight()/2);
  xp = xn = yp = yn = 0;

  universe = new Universe( numberOfPlanets );

  players = new Players( universe );
  universe->actionQueue->scheduleAction( 1000, new UpdatePlayersStatsAction( players) );

  humanPlayer = new HumanPlayer( universe, 0xffffff, 1);
  humanPlayer->setVisible(true);
  universe->claim(0, humanPlayer, 1, 3);
  players->push_back(humanPlayer);
  
  bool vis = gameOptions.getEnemyVisibility();
     
  int hueCounter = rand() % 360;
  for (int i=0;i<numberOfComputerPlayers;i++)
  {
    Player *newPlayer = new ComputerPlayer( universe, HSVtoRGB( hueCounter, 0.9, 0.9 ), 2);
    newPlayer->setVisible(vis);
    universe->claim(0, newPlayer, 1, 3);
    hueCounter = ( hueCounter + 360 / numberOfComputerPlayers ) % 360;
    players->push_back(newPlayer);
  }

  NeutralPlayer *neutral = new NeutralPlayer(universe, 0x808080);
  neutral->setVisible(vis);
  universe->claimRemaining(0, neutral, 3, 2);
  players->push_back(neutral);

  messages = new Messages();
  selection = new Selection();
}

Game::~Game()
{
  delete cursor;
  delete players;
  delete universe;
  delete messages;
  delete selection;
  delete timer;
}

bool
Game::run(bool menuVisible)
{
  if (quit)
    return false;

  // TODO: Find a way to live without the next line.
  setPaused(menuVisible);

	if (timer->isPaused())
    return true;

  moveCursor(xp - xn, yp - yn);

  Uint32 time = getTime();
  // update the universe
  universe->update(time);

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
  if (timer->isPaused())
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
        selection->start(*cursor);
      else
      {
        selection->end();

        if (selection->isEmpty())
          humanPlayer->selectPlanetAt(*cursor);
        else
          universe->planets->sourceSelect(selection, humanPlayer);
      }
      break;
    case GA_SELECT_NEAREST_PLANET:
      if (value)
        humanPlayer->selectNearestPlanet(*cursor);
      break;
    case GA_MOVE_TO_NEAREST_PLANET:
      if (value)
        humanPlayer->moveToNearestPlanet(timer->getTime(), *cursor);
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
  int x = (int) cursor->getXMapped();
  int y = (int) cursor->getYMapped();
  
  x += xrel;
  y += yrel;

  x = max(0, min(x, Canvas::getWidth()));
  y = max(0, min(y, Canvas::getHeight()));
  
  cursor->setXMapped(x);
  cursor->setYMapped(y);
  
  if (selection->isSelecting())
    selection->update(*cursor);
  else
    universe->highlightNearestPlanet(*cursor);
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
  
  stringstream s;
  if (str == 1)
	  s << "Single ship mode";
  else
  {
	  s << "Fleet strength " << str << "%";
  }

  messages->setFleetStrengthMessage( Message(getTime(), s.str(), 0x808080) );
}

void
Game::setPaused(bool p)
{
  if (p ^ timer->isPaused())
    timer->pause();

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

  messages->addMessage(timer->getTime(), Message(getTime(), s.str(), 20000, 0x808080));
}

void
Game::selectAllPlanets()
{
  humanPlayer->selectAllPlanets();
}

void
Game::addMessage(int offset, Message msg)
{
  messages->addMessage( timer->getTime() + offset, msg);
}

Uint32
Game::getTime() const
{
  return timer->getTime();
}

void
Game::render()
{
  Uint32 time = getTime();
  
  universe->renderBackground(time);
  players->render();

  humanPlayer->render();
  selection->render();

  universe->renderForeground(time);

  messages->render(time);

  Canvas::drawCursor(*cursor);
}
