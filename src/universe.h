// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef UNIVERSE_H
#define UNIVERSE_H

class Stars;
class Planet;
class Planets;
class Messages;
class ActionQueue;
class AnimationQueue;

class Universe {
public:
  Stars* stars;
  Planets* planets;
  ActionQueue* actionQueue;
  AnimationQueue* animationQueue;
  Messages* messages;
  
  Planet* currentSelectedPlanet;
  
  Universe();
  Universe( int nrPlanets );
  ~Universe();
  
  void update();
  
  void renderBackground( SDL_Surface* screen );
  void renderForeground( SDL_Surface* screen );
};

#endif
