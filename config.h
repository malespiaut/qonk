// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef CONFIG_H
#define CONFIG_H

#define PI 3.14159265

#include "timer.h"
#include "ReadConf.h"
#include <vector>
#include <string>

static Timer timer;

class Settings {
private:
  	int screenWidth, screenHeight, gameWidth, gameHeight, gameOffsetX;
  	bool fullscreen;
public:
  Settings();
  Settings(int, int, bool);
  void setResolution( int width, int height );
  void setFullscreen( bool b );
  int getScreenWidth();
  int getScreenHeight();
  int getGameWidth();
  int getGameHeight();
  int getGameOffsetX();
  bool getFullscreen();
};

static Settings settings;
//static Config config("qonk.conf");

#endif
