// game.h
// Class which resembles a game and the events which can be triggered
// within it from a player's perspective.
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef GAME_H
#define GAME_H

#include "input.h"

class Universe;
class Players;
class HumanPlayer;
class Message;
class Messages;
class Selection;
class GameOptions;

class Game {

public:
        Game(GameOptions &);

        ~Game();

        bool run(bool);

        bool checkWin();

        bool checkLost();

        void handle(GameAction, int);

        void addMessage(int, Message);

        void render();

        void setPaused(bool);

private:
        void moveCursor(int, int);

        void fleetStrengthUp();

        void fleetStrengthDown();

        void setFleetStrength(int);

        void selectAllPlanets();

        void setEnemyVisibility(bool);

        void selectNearestPlanet(int, int);

        void moveToNearestPlanet(int, int);

        void startSelection(int, int);

        void updateSelection(int, int);

        void endSelection(int, int);

        /** Location of the soft cursor. */
        int x, y;

        /** Current cursor movement forces: x positive, x negative and the same for y. */
        int xp, xn, yp, yn;

        Universe *universe;
        Players *players;
        HumanPlayer *humanPlayer;
        Messages *messages;
        
        Selection *selection;

        enum States { PLAYING, LOST, WON };
        States state;

        bool quit;
        bool nextRound;

        GameOptions &gameOptions;

        int nextNumberOfPlanets;
        int nextNumberOfComputerPlayers;

};

#endif
