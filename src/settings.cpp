// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "readconf.h"
#include "settings.h"

#include <iostream>

int Settings::screenWidth = 0;
int Settings::screenHeight = 0;
int Settings::gameWidth = 0;
int Settings::gameHeight = 0;
int Settings::gameOffsetX = 0;
bool Settings::fullscreen = false;
int Settings::numberOfPlanets = 0;
int Settings::numberOfComputerPlayers = 0;
Config& Settings::config = Config::getInstance();

void
Settings::init() {
	if (config.load())
	  std::cout << "configuration successfully loaded" << std::endl;
	else
	  std::cout << "configuration was not available - using defaults" << std::endl;

	setFullscreen(config.getValueAsBool("fullscreen", false));
	std::cout << "fullscreen: " << fullscreen << std::endl;

	setResolution(config.getValueAsInt("screenWidth", 1024),
		      config.getValueAsInt("screenHeight", 768));
	std::cout << "display size: " << screenWidth << "x" << screenHeight << std::endl;

	setNumberOfPlanets( config.getValueAsInt("numberOfPlanets", 6));
	std::cout << "number of planets: " << numberOfPlanets << std::endl;

	setNumberOfComputerPlayers( config.getValueAsInt("numberOfComputerPlayers", 1));
	std::cout << "number of computer players: " << numberOfComputerPlayers << std::endl;
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
Settings::store()
{
  config.setValue("screenWidth", screenWidth);
  config.setValue("screenHeight", screenHeight);
  config.setValue("fullscreen", fullscreen);
  config.setValue("numberOfComputerPlayers", numberOfComputerPlayers);
  config.setValue("numberOfPlanets", numberOfPlanets);

  if (config.save())
    std::cout << "Setting saved successfully." << std::endl;
  else
    std::cout << "Setting could not be saved." << std::endl;
}
