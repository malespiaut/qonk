// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "config.h"
#include <iostream>
using namespace std;
class Config;

Settings::Settings() {
	//setResolution( 800, 600 );
	//setFullscreen( true );
	
	//Create configuration file if it does not exist
	if (!fileExists("qonk.conf")){
		vector<string> v;
		v.push_back("# Qonk configuration file ");
		v.push_back("# Format: entry = value  (The spaces is optional)\n");
		v.push_back("# Window");
		v.push_back("fullscreen = true");
		v.push_back("width = 1024");
		v.push_back("height = 768\n");
		v.push_back("# Settings for first round");
		v.push_back("numberOfPlanets = 6");
		v.push_back("numberOfComputerPlayers = 1");
		createFile("qonk.conf",v);  
	}
	Config config("qonk.conf");
	//Read settings form qonk.conf
	bool fs = config.getValueBOOL("fullscreen");
	int h = config.getValueINT("height");
	int w = config.getValueINT("width");
	setResolution(w, h );
	setFullscreen( fs );
}

Settings::Settings(int w, int h, bool fs) {
  setResolution( w, h );
  setFullscreen( fs );
}

void
Settings::setResolution( int width, int height ) {
  screenWidth = width;
  screenHeight = height;
  gameWidth = height;
  gameHeight = height;
  gameOffsetX = ( width - height ) / 2;
}

void
Settings::setFullscreen( bool b ) {
  fullscreen = b;
}

int Settings::getScreenWidth() { 
  return screenWidth; 
}

int Settings::getScreenHeight() { 
  return screenHeight; 
}

int Settings::getGameWidth() { 
  return gameWidth; 
}

int Settings::getGameHeight() { 
  return gameHeight; 
}

int Settings::getGameOffsetX() { 
  return gameOffsetX; 
}

bool Settings::getFullscreen() { 
  return fullscreen; 
}
