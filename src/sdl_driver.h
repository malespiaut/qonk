// (c) 2007 Robert Schuster (robertschuster@fsfe.org)
//
// Licensed under the GNU GPL v2 or, at your option, any later version.

#ifndef SDL_DRIVER_H
#define SDL_DRIVER_H

#include <map>

#include <SDL/SDL.h>

#include <guichan.hpp>
#include <guichan/sdl.hpp>

#include "input.h"

class SDLDriver {

  enum MouseState { INITIAL, MOVED, RESET_NEEDED }; 

public:
   SDLDriver(InputHandler &);

   ~SDLDriver();

   void loop();

   gcn::SDLInput *getSDLInput();

   Input &getSensedInput();

   void initActionMap();

   void setSenseMode(bool);

private:
   void input(InputType, int, int, int, int);

   void toggleGrab();

   std::pair< int, int> key(InputType, int, int, int);

   InputHandler &inputHandler;

   std::map< std::pair<int, int>, GameAction > actionMap;

   SDL_Joystick **sticks;

   gcn::SDLInput *sdlInput;

   Input *sensedInput;
};

#endif
