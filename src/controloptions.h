// controloptions.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef CONTROLOPTIONS_H
#define CONTROLOPTION_H

#include <map>

#include "input.h"

class MenuAction;
class Main;

class ControlOptions
{

  Main &main;
  
  std::map < GameAction, MenuAction *> controlActions;

public:
  ControlOptions(Main &);

  MenuAction *getControlAction(GameAction);

  void startSensing();

  void finishSensing(GameAction, bool);
  
  void reset(GameAction);
};

#endif
