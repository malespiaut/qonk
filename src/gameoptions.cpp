#include <vector>
#include <sstream>

#include <SDL/SDL.h>

#include <guichan.hpp>

#include "ui/menuaction.h"

#include "settings.h"
#include "main.h"

#include "gameoptions.h"

using namespace std;
using namespace gcn;

GameOptions::GameOptions()
  : players(1), planets(2), enemyVisibilityAction(0),
    playersAction(0), planetsAction(0), startSinglePlayerGameAction(0)
{
  setPlanets(Settings::getNumberOfPlanets());
  setPlayers(Settings::getNumberOfComputerPlayers());
  setEnemyVisibility(Settings::getEnemyVisibility());
}

void
GameOptions::setEnemyVisibility(bool b)
{
  enemyVisibility = b;

  if (enemyVisibilityAction)
    enemyVisibilityAction->update();
}

void
GameOptions::toggleEnemyVisibility()
{
  setEnemyVisibility(!enemyVisibility);
}

class EnemyVisibilityAction : public MenuAction
{
  GameOptions &gameOptions;
  CheckBox *check;

  public:
    EnemyVisibilityAction(GameOptions &go)
      : gameOptions(go) { }

    virtual gcn::Widget *getWidget(string s)
    {
      return check = new CheckBox(s, gameOptions.getEnemyVisibility());
    }

    void invoke()
    {
      gameOptions.toggleEnemyVisibility();
    }

    void previous()
    {
      invoke();
    }

    void next()
    {
      invoke();
    }

    void update()
    {
      check->setMarked(gameOptions.getEnemyVisibility());
    }
};

MenuAction *
GameOptions::getEnemyVisibilityAction()
{
  if (!enemyVisibilityAction)
    enemyVisibilityAction = new EnemyVisibilityAction(*this);

  return enemyVisibilityAction;
}

void
GameOptions::setPlanets(int p)
{
  planets = p;

  if (planets < 2)
    planets = 2;

  if (planets < players + 1)
    planets = players + 1;

  if (planetsAction)
    planetsAction->update();
}

class PlanetsAction : public MenuAction
{
  GameOptions &gameOptions;
  Label *label;
  string baseCaption;

  public:
    PlanetsAction(GameOptions &go)
      : gameOptions(go) { }

    virtual gcn::Widget *getWidget(string s)
    {
      baseCaption = s;
      stringstream ss;
      ss << baseCaption;
      ss << " ";
      ss << gameOptions.getPlanets();

      return label = new Label(ss.str());
    }

    void invoke()
    {
      next();
    }

    void next()
    {
      gameOptions.setPlanets(gameOptions.getPlanets() + 1);
    }

    void previous()
    {
      gameOptions.setPlanets(gameOptions.getPlanets() - 1);
    }

    void update()
    {
      stringstream ss;
      ss << baseCaption;
      ss << " ";
      ss << gameOptions.getPlanets();

      label->setCaption(ss.str());
      label->adjustSize();
    }
};

MenuAction *
GameOptions::getPlanetsAction()
{
  if (!planetsAction)
    planetsAction = new PlanetsAction(*this);
  
  return planetsAction;
}

void
GameOptions::setPlayers(int p)
{
  players = p;

  if (players <= 0)
    players = 1;

  if (players > planets - 1)
    players = planets - 1;

  if (playersAction)
    playersAction->update();
}

class PlayersAction : public MenuAction
{
  GameOptions &gameOptions;
  Label *label;
  string baseCaption;

  public:
    PlayersAction(GameOptions &go)
      : gameOptions(go) { }

    virtual gcn::Widget *getWidget(string s)
    {
      baseCaption = s;
      stringstream ss;
      ss << baseCaption;
      ss << " ";
      ss << gameOptions.getPlayers();

      return label = new Label(ss.str());
    }

    void invoke()
    {
      next();
    }

    void next()
    {
      gameOptions.setPlayers(gameOptions.getPlayers() + 1);
    }

    void previous()
    {
      gameOptions.setPlayers(gameOptions.getPlayers() - 1);
    }

    void update()
    {
      stringstream ss;
      ss << baseCaption;
      ss << " ";
      ss << gameOptions.getPlayers();

      label->setCaption(ss.str());
      label->adjustSize();
    }

};

MenuAction *
GameOptions::getPlayersAction()
{
  if (!playersAction)
    playersAction = new PlayersAction(*this);

  return playersAction;
}

class StartSinglePlayerGameAction : public MenuAction
{
  Main &main;
  GameOptions &gameOptions;

  public:
    StartSinglePlayerGameAction(Main &m, GameOptions &go)
      : main(m), gameOptions(go) { }

    void invoke()
    {
      main.startSinglePlayerGame(gameOptions);
    }
};

MenuAction *
GameOptions::getStartSinglePlayerGameAction(Main &main)
{
  if (!startSinglePlayerGameAction)
    startSinglePlayerGameAction = new StartSinglePlayerGameAction(main, *this);

  return startSinglePlayerGameAction;
}
