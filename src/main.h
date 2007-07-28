// main.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef MAIN_H
#define MAIN_H

#include "input.h"

class Game;
class VideoOptions;
class GameOptions;
class SDLDriver;
class MenuManager;

class Main : public InputHandler
{

  public:
    Main();
    ~Main();

    void startSinglePlayerGame(int, int);

    void startSinglePlayerGame(GameOptions &);

    void showMenu();

    void applyVideoOptions(VideoOptions &);

    void startSensing();

    void finishSensing(GameAction, bool);
    
    void resetGameAction(GameAction);

    void quit();

    void handle(GameAction, int);

    void run();

    SDLDriver &getSDLDriver() const { return *driver; }

  private:
    void renderFPS();

    int lastTicks, fpsCounter, fps;

    Game *game;
 
    MenuManager *menuManager;

    SDLDriver *driver;

    bool running;

};

#endif
