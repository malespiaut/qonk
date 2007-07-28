// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>
#include <map>

#include <SDL/SDL.h>

class Universe;
class Game;

class Message {
private:
  Uint8 r, g, b;
  Uint32 displayTime;
  std::string message;
public:
  Message( std::string message, Uint32 displayTime, Uint8 r, Uint8 g, Uint8 b );
  Message( std::string message, Uint32 displayTime, Uint32 color );
  Message( std::string message, Uint8 r, Uint8 g, Uint8 b );
  Message( std::string message, Uint32 color );
  void render(int &x, int &y, int time );
  Uint32 getDisplayTime();
};

class Messages : std::multimap< Uint32, Message > {
//Begin Jacobsen
private:
	Message fleetStrengthMessage;
//End Jacobsen
public:
  Messages();
  ~Messages();
  void addMessage( Uint32 time, Message m );
  void cleanup();
  void render();
  
  //Begin Jacobsen
  void setFleetStrengthMessage( Message m );
  //End Jacobsen
};

void MSGwon(Game *, int, int);
void MSGlost(Game *);
void MSGstart(Game *);
#endif
