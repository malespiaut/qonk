// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>

#include "lisp/lisp.hpp"
#include "lisp/writer.hpp"

#include "input.h"

class Settings {
private:
  static const int REQUIRED_CONFIG_FILE_VERSION = 2;
  static const int CURRENT_CONFIG_FILE_VERSION = 2;

  static std::string fileName;

  static int screenWidth, screenHeight;
  static bool fullscreen;
  static bool enemyVisibility;

  static int numberOfPlanets;
  static int numberOfComputerPlayers;

  static Input inputMap[GA_COUNT][2];

  static void writeInput(lisp::Writer *, const char *, GameAction);

  static void readInput(const lisp::Lisp *, const char *, GameAction);
  static void setupDefaultActionMap();
  static void set(GameAction, InputType, int, int, int);
  
  /** Iterates through the input mapping and unsets all
   * where the given input occurs.
   * 
   * This makes sure an input is not bound multiple times.
   */
  static void unsetDuplicates(GameAction, InputType, int, int, int);

  static std::string getConfigFilePath();
  static bool fileExists(const std::string&);

  static std::string getInputAsString(Input &);
  

public:
  static void init();

  static void setEnemyVisibility(bool b) { enemyVisibility = b; }
  static bool getEnemyVisibility() { return enemyVisibility; }

  static void setFullscreen( bool b ) { fullscreen = b; }
  static bool getFullscreen() { return fullscreen; }

  static void setScreenSize(int, int);

  static int getScreenWidth() { return screenWidth; }

  static int getScreenHeight() { return screenHeight; }

  static int getNumberOfPlanets() { return numberOfPlanets; }
  static int setNumberOfPlanets(int p) { numberOfPlanets = p; }

  static int getNumberOfComputerPlayers() { return numberOfComputerPlayers; }
  static int setNumberOfComputerPlayers(int p) { numberOfComputerPlayers = p; }

  static Input getInput(GameAction);
  static Input getAltInput(GameAction);

  static void store();
  
  static std::string getAsString(GameAction);

  static void printInput(char *);

  static void set(GameAction, Input &);
  static void unset(GameAction);

};

#endif
