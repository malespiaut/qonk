// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "actions.h"

class Planet;

class Animation : public Action {
public:
  void execute( const Uint32& time ) { (void)time; }
  virtual void render(Uint32) = 0;
};

class AnimationQueue : public ActionQueue {
public:
  void render(Uint32);
};

class SonarAnimation : public Animation {
private:
  bool circle;
  Planet* planet;
  Uint32 startTime, endTime;
  Uint8 r, g, b;
  int size;
public:
  SonarAnimation();
  SonarAnimation( Planet* planet, const Uint32& color, const int& size, const Uint32& startTime, const Uint32& endTime, bool circle = false );
  void render(Uint32);
};

#endif
