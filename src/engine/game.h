// game.h
// Class which resembles a game and the events which can be triggered
// within it from a player's perspective.
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "../input.h"

class Universe;
class Players;
class HumanPlayer;
class Message;
class Messages;
class Selection;
class GameOptions;
class Timer;
class Coordinate;

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
        
        Uint32 getTime() const;

private:
        void moveCursor(int, int);

        void fleetStrengthUp();

        void fleetStrengthDown();

        void setFleetStrength(int);

        void selectAllPlanets();

        void setEnemyVisibility(bool);

        void selectNearestPlanet(int, int);

        void moveToNearestPlanet(int, int);

        /** Location of the soft cursor. */
        Coordinate *cursor;

        /** Current cursor movement forces: x positive, x negative and the same for y. */
        int xp, xn, yp, yn;

        Universe *universe;
        HumanPlayer *humanPlayer;
        Messages *messages;
        Timer *timer;
        Players *players;
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
