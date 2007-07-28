// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Stars;
class Planet;
class Planets;
class Player;
class ActionQueue;
class AnimationQueue;

class Universe {
public:
  Stars* stars;
  Planets* planets;
  ActionQueue* actionQueue;
  AnimationQueue* animationQueue;
  
  Planet* currentSelectedPlanet;
  
  Universe();
  Universe( int nrPlanets );
  ~Universe();

  void highlightNearestPlanet(int x, int y);
  
  void update(Uint32);
  
  void renderBackground(Uint32);
  void renderForeground(Uint32);
  
  void claim(Uint32, Player *, int, int);
  void claimRemaining(Uint32, Player *, int, int);
};

#endif
