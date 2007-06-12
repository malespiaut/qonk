// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef SETTINGS_H
#define SETTINGS_H

#include "timer.h"
#include "readconf.h"

#include <vector>
#include <string>

class Settings {
private:
  static int screenWidth, screenHeight, gameWidth, gameHeight, gameOffsetX;
  static bool fullscreen;

  static int numberOfPlanets;
  static int numberOfComputerPlayers;

  static Config& config;

public:
  static void init();

  static void setFullscreen( bool b ) { fullscreen = b; }
  static bool getFullscreen() { return fullscreen; }

  static void setResolution(int, int);
  static int getScreenWidth() { return screenWidth; }
  static int getScreenHeight() { return screenHeight; }
  static int getGameWidth() { return gameWidth; }
  static int getGameHeight() { return gameHeight; }
  static int getGameOffsetX() { return gameOffsetX; }

  static int getNumberOfPlanets() { return numberOfPlanets; }
  static int setNumberOfPlanets(int p) { numberOfPlanets = p; }

  static int getNumberOfComputerPlayers() { return numberOfComputerPlayers; }
  static int setNumberOfComputerPlayers(int p) { numberOfComputerPlayers = p; }

  static void store();
};

#endif
