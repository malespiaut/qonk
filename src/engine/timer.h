// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef TIMER_H
#define TIMER_H

#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <sstream>

class Timer {
private:
  Uint32 startTime, pauseTime;
  bool paused;
public:
  Timer();
  
  void start();
  void pause(); // this function does both pause and resume the clock
  
  bool isPaused();
  Uint32 getTime();
  std::string getTimeMMSSHH();
  static std::string getTimeMMSS(Uint32);

};

#endif
