// gameoptions.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef GAMEOPTIONS_H
#define GAMEOPTION_H

#include <vector>

class Main;
class MenuAction;

/**
 * GameOptions takes all the possible options for a single player game and
 * allows modifying them through MenuActions.
 *
 */
class GameOptions
{
  bool enemyVisibility;

  int players;
  int planets;

  MenuAction *enemyVisibilityAction;
  MenuAction *playersAction;
  MenuAction *planetsAction;
  MenuAction *startSinglePlayerGameAction;

  public:
    GameOptions();

    void setPlayers(int);
    int getPlayers() const { return players; }

    void setPlanets(int);
    int getPlanets() const { return planets; }

    void setEnemyVisibility(bool);
    void toggleEnemyVisibility();
    bool getEnemyVisibility() const { return enemyVisibility; }

    MenuAction *getEnemyVisibilityAction();

    MenuAction *getPlayersAction();

    MenuAction *getPlanetsAction();

    MenuAction *getStartSinglePlayerGameAction(Main &);
};

#endif
