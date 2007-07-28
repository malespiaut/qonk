// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef ACTIONS_H
#define ACTIONS_H

#include <SDL/SDL.h>
#include <map>

class ActionQueue;

class Action {
private:
  ActionQueue* actionQueue;
protected:
  int actionID;
public:
  Action();
  virtual void execute( const Uint32& time ) = 0;
  virtual ~Action() {}
  void setActionQueue( ActionQueue* actionQueue );
  ActionQueue* getActionQueue() const;
  int getActionID() { return actionID; }
};

class ActionQueue : public std::multimap< Uint32, Action* > {
private:
  Uint32 time;
public:
  ActionQueue(); 
  void scheduleAction( Uint32 time, Action* action );
  
  Uint32 timeOfNextEvent();
  
  void executeNextEvent();
  void executeAllEvents();
  void executeEventsBefore( Uint32 time );
  void executePastEvents();
};

#endif
