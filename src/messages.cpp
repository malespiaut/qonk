// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "messages.h"
#include "timer.h"
#include "settings.h"
#include "coordinate.h"
#include "extensions.h"
#include "game.h"
#include "canvas.h"

using namespace std;

Message::Message( Uint32 time, string message, Uint32 displayTime, Uint8 r, Uint8 g, Uint8 b ) {
  this->message = string( "[" ) + Timer::getTimeMMSS(time) + "] " + message;
  this->displayTime= displayTime;
  this->r = r;
  this->g = g;
  this->b = b;
}

Message::Message( Uint32 time, string message, Uint32 displayTime, Uint32 color ) {
  this->message = string( "[" ) + Timer::getTimeMMSS(time) + "] " + message;
  this->displayTime = displayTime;
  this->r = getGreen( color );
  this->g = getRed( color );
  this->b = getBlue( color );
}

Message::Message( Uint32 time, string message, Uint8 r, Uint8 g, Uint8 b ) {
  this->message = string( "[" ) + Timer::getTimeMMSS(time) + "] " + message;
  this->displayTime= 10000;
  this->r = r;
  this->g = g;
  this->b = b;
}

Message::Message( Uint32 time, string message, Uint32 color ) {
  this->message = string( "[" ) + Timer::getTimeMMSS(time) + "] " + message;
  this->displayTime = 10000;
  this->r = getGreen( color );
  this->g = getRed( color );
  this->b = getBlue( color );
}

void
Message::render( int &x, int &y, int time) {
  if( time > 0 ) {
    
    // default parameters for showing a message
    
    int dx = Canvas::getFontHeight();
    int alpha = 255;
    
    // fading in a message
    if( time < 500 ) {
      dx = (int)( ( dx * time ) / 600.0 );
      alpha = (int)( ( alpha * time ) / 600.0 );
    }
    
    // fading out a message
    if( time > displayTime - 500 ) {
      int timeleft = displayTime - time;
      dx = (int)( ( dx * timeleft ) / 600.0 );
      alpha = (int)( ( alpha * timeleft ) / 600.0 );
    }
    
    // do we still show ourselves?
    if( time > displayTime ) {
    
      dx = 0;
      alpha = 0;
      
    } else {
    
      Canvas::drawText( x, y - dx, message.c_str(), r, g, b, alpha );
      
    }
    
    y -= dx;
  } 
   // Begin Jacobsen
  if ( time == -1 )
	  Canvas::drawText( x, y , message.c_str(), r, g, b, 255 );
    // End jacobsen
}

Uint32
Message::getDisplayTime() {
  return displayTime;
}
//Jacobsen - added init of fleetStrengthMessage
Messages::Messages() : fleetStrengthMessage(0, "Fleet strength 50%",0x808080) {
}

Messages::~Messages() {
  clear();
}

void
Messages::addMessage( Uint32 time, Message m ) {
  insert( pair< Uint32, Message>( time, m ) );
}

void
Messages::cleanup(Uint32 time) {
  while( ( size() > 0 ) && ( begin()->first + begin()->second.getDisplayTime() < time ) )
    erase( begin() );
}

void
Messages::render(Uint32 time) {
  cleanup(time);
  int x = 12; 
  int y = Settings::getGameHeight() - 28;
  for( reverse_iterator i = rbegin(); i != rend(); i++ ) {
    i->second.render(x, y, (int)time - (int)i->first );
  }
  
  //Begin Jacobsen
  x = Settings::getGameWidth() - 80; 
  y = Settings::getGameHeight() - 28;
  fleetStrengthMessage.render(x,y,-1);
  //End Jacobsen
}

//Begin Jacobsen
void Messages::setFleetStrengthMessage( Message m ) {
	fleetStrengthMessage = m;
}
//End Jacobsen

void
MSGwon(Game *game, int nextPlanets, int nextComputerPlayers){
  Uint32 time = game->getTime();
	game->addMessage(200, Message(time, "You have QONKuered the solar system! You won!", 20000, 0xffffff ) );
	stringstream s;
	s << "Try again, with " << nextPlanets << " planets and " << nextComputerPlayers << " AI players!";
	game->addMessage(400, Message(time, s.str(), 20000, 0x808080 ) );

  s.str("");
  s << "Use [" << Settings::getAsString(GA_NEXT_ROUND);
  s << "] to start the next round";

	game->addMessage(600, Message(time, s.str(), 40000, 0x808080 ) );
}

void 
MSGlost(Game *game) {
	Uint32 time = game->getTime();
  game->addMessage(0, Message(time, "GAME OVER!!! You lost all your planets and ships, you are dead!", 20000, 0xffffff ) );
	game->addMessage(200, Message(time, "Press [R] ...", 40000, 0x808080 ) );
}

void 
MSGstart(Game *game) {
  Uint32 time = game->getTime();
  game->addMessage( 000, Message(time, "Let's QONK! Kick the AI players out!", 15000, 0xffffff ) );
  game->addMessage( 200, Message(time, "[Left click and drag] for multi select, [Middle click] for single select.", 10000, 0x808080 ) );
  game->addMessage( 400, Message(time, "[Right click] to send ships.", 10000, 0x808080 ) );
  game->addMessage( 600, Message(time, "[P]ause,[A]ll planets,[R]estart.", 10000, 0x808080 ) );
  game->addMessage( 700, Message(time, "[Escape] to quit", 10000, 0x808080 ) );
  game->addMessage( 800, Message(time, "[1]-[0] fleet selection (10%-100%)", 10000, 0x808080 ) );
  game->addMessage(1000, Message(time, "Initial fleet selection is 50%", 10000, 0x808080 ) );
}
