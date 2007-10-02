// Copyright 2005 by Anthony Liekens anthony@liekens.net
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

#include <SDL/SDL.h>

#include "lisp/lisp.hpp"
#include "lisp/writer.hpp"
#include "lisp/parser.hpp"

#include "settings.h"
#include "input.h"

using namespace lisp;
using namespace std;

string Settings::fileName = getConfigFilePath() + "qonk.config";

int Settings::screenWidth = 0;
int Settings::screenHeight = 0;
bool Settings::fullscreen = false;

int Settings::numberOfPlanets = 6;
int Settings::numberOfComputerPlayers = 1;
bool Settings::enemyVisibility = false;

Input Settings::inputMap[GA_COUNT][2];

string
Settings::getConfigFilePath(){
	string pathname = "";
#ifdef WINDOWS
	// Do nothing. Falls back to application directory.
#elif MAC_OSX
	// Get from some mysterious directory ...
#else
	// GNU/LINUX/BSD/POSIX/...
	if(getenv("HOME")!=NULL) {
		pathname = getenv("HOME");
		pathname += "/.qonk/";
		mkdir(pathname.c_str(), 0755);
        }
#endif

	return pathname;
}

void
Settings::init() {
  // Ensures that input map is zeroed.
  memset(inputMap, 0, sizeof(inputMap));

  // Those are fixed and not supposed to be changed via a config file.
  set(GA_TOGGLE_GRAB, IT_KEYBOARD, SDLK_g, 0, 0);
  set(GA_LEAVE, IT_KEYBOARD, SDLK_ESCAPE, 0, 0);

  const lisp::Lisp* root = 0;

 	if( !fileExists(fileName))
  {
	  cout << "configuration was not available - using defaults" << endl;
    setScreenSize(800, 600);
    setupDefaultActionMap();
    return;
  }

  try
  {
    Parser parser;
    root = parser.parse(fileName);
  }
  catch(std::exception& e)
  {
    printf("Config file '%s' does not exist, it will be created.\n",
           fileName.c_str());
    delete root;

    return;
  }

  const Lisp *qonkReader = 0;
  if (!(qonkReader = root->getLisp("qonk-config")))
  {
	  cout << "configuration file malformed - using defaults" << endl;
    setScreenSize(800, 600);
    setupDefaultActionMap();

    delete root;

    return;
  }

  int configFileVersion = 0;
  qonkReader->get("configFileVersion", configFileVersion);

  if (configFileVersion < REQUIRED_CONFIG_FILE_VERSION)
  {
	  cout << "configuration file version " << configFileVersion << " not supported - using default values." << endl;
    setScreenSize(800, 600);
    setupDefaultActionMap();

    delete root;
//    delete qonkReader;
    return;
  }

  qonkReader->get("screenWidth", screenWidth);
  qonkReader->get("screenHeight", screenHeight);
  setScreenSize(screenWidth, screenHeight);

  qonkReader->get("fullscreen", fullscreen);

  qonkReader->get("planets", numberOfPlanets);
  qonkReader->get("enemies", numberOfComputerPlayers);
  qonkReader->get("enemiesVisible", enemyVisibility);

  const Lisp *reader = 0;
  reader = qonkReader->getLisp("input");
  if (reader)
  {
    readInput(reader, "screenshot", GA_SCREENSHOT);
    readInput(reader, "pause", GA_PAUSE);
    readInput(reader, "toggleFullscreen", GA_TOGGLE_FULLSCREEN);
    readInput(reader, "restartRound", GA_RESTART_ROUND);
    readInput(reader, "nextRound", GA_NEXT_ROUND);

    readInput(reader, "selectAll", GA_SELECT_ALL);
    readInput(reader, "toggleEnemyVisibility", GA_TOGGLE_ENEMY_VISIBILITY);

    readInput(reader, "selection", GA_SELECTION);

    readInput(reader, "cursorUp", GA_CURSOR_UP);
    readInput(reader, "cursorDown", GA_CURSOR_DOWN);
    readInput(reader, "cursorLeft", GA_CURSOR_LEFT);
    readInput(reader, "cursorRight", GA_CURSOR_RIGHT);

    readInput(reader, "selectNearestPlanet", GA_SELECT_NEAREST_PLANET);
    readInput(reader, "moveToNearestPlanet", GA_MOVE_TO_NEAREST_PLANET);

    readInput(reader, "setFleetStrengthSingle", GA_SET_FLEET_STRENGTH_SINGLE);
    readInput(reader, "fleetStrengthUp", GA_FLEET_STRENGTH_UP);
    readInput(reader, "fleetStrengthDown", GA_FLEET_STRENGTH_DOWN);

    readInput(reader, "setFleetStrength10", GA_SET_FLEET_STRENGTH_10);
    readInput(reader, "setFleetStrength20", GA_SET_FLEET_STRENGTH_20);
    readInput(reader, "setFleetStrength30", GA_SET_FLEET_STRENGTH_30);
    readInput(reader, "setFleetStrength40", GA_SET_FLEET_STRENGTH_40);
    readInput(reader, "setFleetStrength50", GA_SET_FLEET_STRENGTH_50);
    readInput(reader, "setFleetStrength60", GA_SET_FLEET_STRENGTH_60);
    readInput(reader, "setFleetStrength70", GA_SET_FLEET_STRENGTH_70);
    readInput(reader, "setFleetStrength80", GA_SET_FLEET_STRENGTH_80);
    readInput(reader, "setFleetStrength90", GA_SET_FLEET_STRENGTH_90);
    readInput(reader, "setFleetStrength100", GA_SET_FLEET_STRENGTH_100);
  }
  else
  {
    cerr << "Input configuration not present or damaged - using defaults" << endl;
    setupDefaultActionMap();
  }

  delete root;
}

void
Settings::setScreenSize( int width, int height ) {
  screenWidth = width;
  screenHeight = height;
}

void
Settings::store()
{
  Writer *writer = new Writer(fileName);

  try
  {
    writer->beginList("qonk-config");
    writer->writeComment("If the game's supported config file version is higher than this number the configuration is discarded.");
    writer->write("configFileVersion", CURRENT_CONFIG_FILE_VERSION);

    writer->writeComment("General display properties");
    writer->write("screenWidth", screenWidth);
    writer->write("screenHeight", screenHeight);
    writer->write("fullscreen", fullscreen);

    writer->writeComment("General gameplay properties");
    writer->write("planets", numberOfPlanets);
    writer->write("enemies", numberOfComputerPlayers);
    writer->write("enemiesVisible", enemyVisibility);

    writer->beginList("input");

    writeInput(writer, "screenshot", GA_SCREENSHOT);
    writeInput(writer, "pause", GA_PAUSE);
    writeInput(writer, "toggleFullscreen", GA_TOGGLE_FULLSCREEN);
    writeInput(writer, "restartRound", GA_RESTART_ROUND);
    writeInput(writer, "nextRound", GA_NEXT_ROUND);

    writeInput(writer, "selectAll", GA_SELECT_ALL);
    writeInput(writer, "toggleEnemyVisibility", GA_TOGGLE_ENEMY_VISIBILITY);

    writeInput(writer, "selection", GA_SELECTION);

    writeInput(writer, "cursorUp", GA_CURSOR_UP);
    writeInput(writer, "cursorDown", GA_CURSOR_DOWN);
    writeInput(writer, "cursorLeft", GA_CURSOR_LEFT);
    writeInput(writer, "cursorRight", GA_CURSOR_RIGHT);

    writeInput(writer, "selectNearestPlanet", GA_SELECT_NEAREST_PLANET);
    writeInput(writer, "moveToNearestPlanet", GA_MOVE_TO_NEAREST_PLANET);

    writeInput(writer, "setFleetStrengthSingle", GA_SET_FLEET_STRENGTH_SINGLE);
    writeInput(writer, "fleetStrengthUp", GA_FLEET_STRENGTH_UP);
    writeInput(writer, "fleetStrengthDown", GA_FLEET_STRENGTH_DOWN);

    writeInput(writer, "setFleetStrength10", GA_SET_FLEET_STRENGTH_10);
    writeInput(writer, "setFleetStrength20", GA_SET_FLEET_STRENGTH_20);
    writeInput(writer, "setFleetStrength30", GA_SET_FLEET_STRENGTH_30);
    writeInput(writer, "setFleetStrength40", GA_SET_FLEET_STRENGTH_40);
    writeInput(writer, "setFleetStrength50", GA_SET_FLEET_STRENGTH_50);
    writeInput(writer, "setFleetStrength60", GA_SET_FLEET_STRENGTH_60);
    writeInput(writer, "setFleetStrength70", GA_SET_FLEET_STRENGTH_70);
    writeInput(writer, "setFleetStrength80", GA_SET_FLEET_STRENGTH_80);
    writeInput(writer, "setFleetStrength90", GA_SET_FLEET_STRENGTH_90);
    writeInput(writer, "setFleetStrength100", GA_SET_FLEET_STRENGTH_100);

    writer->endList("input");

    writer->endList("qonk-config");
  }
  catch (exception &)
  {
    cerr << "Could not write config file." << endl;
  }

  delete writer;
}

void
Settings::readInput(const lisp::Lisp* r,
                       const char *node,
                       GameAction action)
{
    string inputTypeName;
    const char *which[] = { "normal", "alternate" };

    const Lisp* nodeReader = r->getLisp(node);
    if (!nodeReader)
      return;

    for(int i=0;i<2;i++)
    {
    InputType it=IT_NONE;
    // Every unused id variable *must* be set to
    // something different than -1. Otherwise
    // the restored mapping will not be applied.
    int id0 = -1, id1 = -1, id2 = -1;

    const Lisp *subReader = nodeReader->getLisp(which[i]);
    if (!subReader)
      break;

    subReader->get("type", inputTypeName);
    if (inputTypeName == "keyboard")
    {
        it = IT_KEYBOARD;
        subReader->get("key", id0);
        id1 = id2 = 0;
    }
    else if (inputTypeName == "stickaxis")
    {
        it = IT_STICKMOTION;
        subReader->get("stick", id0);
        subReader->get("axis", id1);
        subReader->get("direction", id2);
    }
    else if (inputTypeName == "stickbutton")
    {
        it = IT_STICKBUTTON;
        subReader->get("stick", id0);
        subReader->get("button", id1);
        id2 = 0;
    }
    else if (inputTypeName == "stickhat")
    {
        it = IT_STICKHAT;
        // TODO: Implement me
    }
    else if (inputTypeName == "mouseaxis")
    {
        it = IT_MOUSEMOTION;
        subReader->get("axis", id0);
        subReader->get("direction", id1);
        id2 = 0;
    }
    else if (inputTypeName == "mousebutton")
    {
        it = IT_MOUSEBUTTON;
        subReader->get("button", id0);
        id1 = id2 = 0;
    }

    if (id0 != -1 && id1 != -1 && id2 != -1)
        set(action, it, id0, id1, id2);

    }

}

void
Settings::unset(GameAction ga)
{
  // Deletes the 2nd entry if it exists or
  // the first if not.
  if (inputMap[ga][1].inputType != IT_NONE)
    inputMap[ga][1].inputType = IT_NONE;
  else
    inputMap[ga][0].inputType = IT_NONE;
}

void
Settings::unsetDuplicates (GameAction ga, InputType it, int id0, int id1, int id2)
{
  for (int cga = GA_FIRST; cga < GA_COUNT; cga++)
  {
    if (cga != ga)
    {
      // If the input occurs in any other mapping
      // delete it properly from there.
      
      if (inputMap[cga][1].inputType == it
          && inputMap[cga][1].id0 == id0
          && inputMap[cga][1].id1 == id1
          && inputMap[cga][1].id2 == id2)
      {
        // Deletes it from the 2nd entry.
        inputMap[cga][1].inputType = IT_NONE;
      }

      if (inputMap[cga][0].inputType == it
          && inputMap[cga][0].id0 == id0
          && inputMap[cga][0].id1 == id1
          && inputMap[cga][0].id2 == id2)
      {
        // Deletes it from the 1st entry and
        // shifts the 2nd to the first.
        inputMap[cga][0] = inputMap[cga][1];
        inputMap[cga][1].inputType = IT_NONE;
      }
    }
  }
}

void
Settings::set(GameAction ga, InputType it, int id0, int id1, int id2)
{
  // Do not change anything if the sensed input already
  // exists for the game action. 
  if (inputMap[ga][0].inputType == it
          && inputMap[ga][0].id0 == id0
          && inputMap[ga][0].id1 == id1
          && inputMap[ga][0].id2 == id2
          || inputMap[ga][1].inputType == it
          && inputMap[ga][1].id0 == id0
          && inputMap[ga][1].id1 == id1
          && inputMap[ga][1].id2 == id2)
    return;
  
  // Removes the input from all mappings where it occurs.
  unsetDuplicates(ga, it, id0, id1, id2);
    
  // Setting a new value is implemented by shifting the value to the
  // right and applying the new values in the 1st location.
   
  inputMap[ga][1] = inputMap[ga][0];

  // New input is always placed on the first location.	
  Input &im = inputMap[ga][0];

  im.inputType = it;
  im.id0 = id0;
  im.id1 = id1;
  im.id2 = id2;
}

/**
 * Connects the given Input with the GameAction.
 *
 * TODO: In the future the return value may indicate whether
 * the input is already in use (conflict) and was rejected.
 */
void
Settings::set(GameAction ga, Input &input)
{
  set(ga, input.inputType, input.id0, input.id1, input.id2);
}

void
Settings::writeInput(lisp::Writer *writer, const char *node, GameAction action)
{
    writer->beginList(node);

    for (int i=0;i<2;i++)
    {
    const Input input = inputMap[action][i];
    const char *which[] =  { "normal", "alternate" };

    if (input.inputType != IT_NONE)
    {
    writer->beginList(which[i]);

    switch (input.inputType)
    {
    case IT_KEYBOARD:
        writer->write("type", "keyboard");
        writer->write("key", input.id0);
        break;
    case IT_STICKMOTION:
        writer->write("type", "stickaxis");
        writer->write("stick", input.id0);
        writer->write("axis", input.id1);
        writer->writeComment("0 is negative/left/up, 1 is positive/right/down");
        writer->write("direction", input.id2);
        break;
    case IT_STICKBUTTON:
        writer->write("type", "stickbutton");
        writer->write("stick", input.id0);
        writer->write("button", input.id1);
        break;
    case IT_STICKHAT:
        // TODO: Implement me
        break;
    case IT_MOUSEMOTION:
        writer->write("type", "mouseaxis");
        writer->write("axis", input.id0);
        writer->writeComment("0 is negative/left/up, 1 is positive/right/down");
        writer->write("direction", input.id1);
        break;
    case IT_MOUSEBUTTON:
        writer->write("type", "mousebutton");
        writer->writeComment("0 is left, 1 is middle, 2 is right, 3 is wheel up, 4 is wheel down");
        writer->writeComment("other values denote auxillary buttons");
        writer->write("button", input.id0);
        break;
    }
    writer->endList(which[i]);
    }

    }

    writer->endList(node);
}

void
Settings::setupDefaultActionMap()
{
  set(GA_SCREENSHOT, IT_KEYBOARD, SDLK_F12, 0, 0);
  set(GA_PAUSE, IT_KEYBOARD, SDLK_p, 0, 0);
  set(GA_TOGGLE_FULLSCREEN, IT_KEYBOARD, SDLK_f, 0, 0);

  set(GA_RESTART_ROUND, IT_KEYBOARD, SDLK_r, 0, 0);
  set(GA_NEXT_ROUND, IT_KEYBOARD, SDLK_n, 0, 0);

  set(GA_SELECT_ALL, IT_KEYBOARD, SDLK_a, 0, 0);
  set(GA_TOGGLE_ENEMY_VISIBILITY, IT_KEYBOARD, SDLK_e, 0, 0);

  set(GA_SELECTION, IT_MOUSEBUTTON, SDL_BUTTON_LEFT, 0, 0);
  set(GA_SELECTION, IT_KEYBOARD, SDLK_LSHIFT, 0, 0);

  set(GA_SELECT_NEAREST_PLANET, IT_MOUSEBUTTON, SDL_BUTTON_MIDDLE, 0, 0);
  set(GA_SELECT_NEAREST_PLANET, IT_KEYBOARD, SDLK_TAB, 0, 0);

  set(GA_MOVE_TO_NEAREST_PLANET, IT_MOUSEBUTTON, SDL_BUTTON_RIGHT, 0, 0);
  set(GA_MOVE_TO_NEAREST_PLANET, IT_KEYBOARD, SDLK_LCTRL, 0, 0);

  set(GA_CURSOR_LEFT, IT_MOUSEMOTION, 0, AD_NEGATIVE, 0);
  set(GA_CURSOR_LEFT, IT_KEYBOARD, SDLK_LEFT, 0, 0);

  set(GA_CURSOR_RIGHT, IT_MOUSEMOTION, 0, AD_POSITIVE, 0);
  set(GA_CURSOR_RIGHT, IT_KEYBOARD, SDLK_RIGHT, 0, 0);

  set(GA_CURSOR_UP, IT_MOUSEMOTION, 1, AD_NEGATIVE, 0);
  set(GA_CURSOR_UP, IT_KEYBOARD, SDLK_UP, 0, 0);

  set(GA_CURSOR_DOWN, IT_MOUSEMOTION, 1, AD_POSITIVE, 0);
  set(GA_CURSOR_DOWN, IT_KEYBOARD, SDLK_DOWN, 0, 0);

/* preliminary: cursor control with joystick - works good
  set(GA_SELECTION, IT_STICKBUTTON, 0, 1, 0);
  set(GA_SELECT_NEAREST_PLANET, IT_STICKBUTTON, 0, 2, 0);
  set(GA_MOVE_TO_NEAREST_PLANET, IT_STICKBUTTON, 0, 3, 0);
  set(GA_CURSOR_LEFT, IT_STICKMOTION, 0, 0, AD_NEGATIVE);
  set(GA_CURSOR_RIGHT, IT_STICKMOTION, 0, 0, AD_POSITIVE);
  set(GA_CURSOR_UP, IT_STICKMOTION, 0, 1, AD_NEGATIVE);
  set(GA_CURSOR_DOWN, IT_STICKMOTION, 0, 1, AD_POSITIVE);
*/

  set(GA_FLEET_STRENGTH_UP, IT_MOUSEBUTTON, SDL_BUTTON_WHEELUP, 0, 0);
  set(GA_FLEET_STRENGTH_UP, IT_KEYBOARD, SDLK_PLUS, 0, 0);

  set(GA_FLEET_STRENGTH_DOWN, IT_MOUSEBUTTON, SDL_BUTTON_WHEELDOWN, 0, 0);
  set(GA_FLEET_STRENGTH_DOWN, IT_KEYBOARD, SDLK_MINUS, 0, 0);

  set(GA_SET_FLEET_STRENGTH_SINGLE, IT_KEYBOARD, SDLK_s, 0, 0);
  set(GA_SET_FLEET_STRENGTH_10, IT_KEYBOARD, SDLK_1, 0, 0);
  set(GA_SET_FLEET_STRENGTH_20, IT_KEYBOARD, SDLK_2, 0, 0);
  set(GA_SET_FLEET_STRENGTH_30, IT_KEYBOARD, SDLK_3, 0, 0);
  set(GA_SET_FLEET_STRENGTH_40, IT_KEYBOARD, SDLK_4, 0, 0);
  set(GA_SET_FLEET_STRENGTH_50, IT_KEYBOARD, SDLK_5, 0, 0);
  set(GA_SET_FLEET_STRENGTH_60, IT_KEYBOARD, SDLK_6, 0, 0);
  set(GA_SET_FLEET_STRENGTH_70, IT_KEYBOARD, SDLK_7, 0, 0);
  set(GA_SET_FLEET_STRENGTH_80, IT_KEYBOARD, SDLK_8, 0, 0);
  set(GA_SET_FLEET_STRENGTH_90, IT_KEYBOARD, SDLK_9, 0, 0);
  set(GA_SET_FLEET_STRENGTH_100, IT_KEYBOARD, SDLK_0, 0, 0);
}

Input
Settings::getInput(GameAction ga)
{
  return inputMap[ga][0];
}

Input
Settings::getAltInput(GameAction ga)
{
  return inputMap[ga][1];
}

string
Settings::getInputAsString(Input &input)
{
    char msg[512];
    std::ostringstream stm;
    
    switch (input.inputType)
    {
    case IT_KEYBOARD:
        snprintf(msg, sizeof(msg), ("%s"), SDL_GetKeyName((SDLKey) input.id0));
        break;
    case IT_STICKMOTION:
        snprintf(msg, sizeof(msg), ("joy %d axis %d  %c"),
                 input.id0, input.id1, (input.id2 == AD_NEGATIVE) ? '-' : '+');
        break;
    case IT_STICKBUTTON:
        snprintf(msg, sizeof(msg), ("joy %d button %d"), input.id0, input.id1);
        break;
    case IT_STICKHAT:
        snprintf(msg, sizeof(msg), ("joy %d hat %d"), input.id0, input.id1);
        break;
    case IT_MOUSEBUTTON:
        snprintf(msg, sizeof(msg), ("mouse button %d"), input.id0);
        break;
    case IT_MOUSEMOTION:
        // Axis 0 and 1 represent x and y. Thats for sure. If we get higher
        // numbers just print those instead.
        if (input.id0 < 2)
          snprintf(msg, sizeof(msg), ("mouse %c axis %c"),
                   ((input.id0 == 0) ? 'x' : 'y'), ((input.id1 == AD_NEGATIVE) ? '-' : '+'));
        else
          snprintf(msg, sizeof(msg), ("mouse axis %d %c"),
                   input.id0, ((input.id1 == AD_NEGATIVE) ? '-' : '+'));

        break;
    default:
        snprintf(msg, sizeof(msg), ("Invalid"));
    }
    
    stm << msg;
    
    return stm.str();
}

string
Settings::getAsString(GameAction ga)
{
  Input input = inputMap[ga][0];
  if (input.inputType != IT_NONE)
  {
    stringstream s;
    s << getInputAsString(input);

    input = inputMap[ga][1];
    if (input.inputType != IT_NONE)
      s << " or " << getInputAsString(input);

    return s.str();
  }
  else
  {
    return string("undefined");
  }
}

void
Settings::printInput(char *msg)
{
  cerr << msg << ":" << endl;

  int size = GA_FIRST + GA_COUNT;
  for (int i=GA_FIRST; i<size;i++)
  {
    Input input = inputMap[i][0];
    cerr << "normal action: " << i << " - input.inputType: " << input.inputType << " - input.id0: " << input.id0 << endl;
    input = inputMap[i][1];
    cerr << "   alt action: " << i << " - input.inputType: " << input.inputType << " - input.id0: " << input.id0 << endl << endl;
  }
}

bool 
Settings::fileExists( const std::string& fileName ) {
	std::ifstream infile( fileName.c_str() );
	bool result = infile.good();
	infile.close();

	return result;
}
