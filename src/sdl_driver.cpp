// (c) 2007 Robert Schuster (robertschuster@fsfe.org)
//
// Licensed under the GNU GPL v2 or, at your option, any later version.

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <cstdio>

#include "sdl_driver.h"
#include "engine/canvas.h"
#include "input.h"
#include "settings.h"

#include <iostream>
#include <map>

using namespace std;

const int DEADZONE_MOUSE = 1;
const int DEADZONE_STICK = 1000;

SDLDriver::SDLDriver(InputHandler &ih) : inputHandler(ih), sensedInput(0)
{
    /* Init SDL */
    if( SDL_Init( SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO ) < 0 ) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        exit( 1 );
    }
    SDL_EnableUNICODE(SDL_ENABLE);

    sdlInput = new gcn::SDLInput();

    atexit( SDL_Quit );

    if ( TTF_Init() < 0 ) {
        std::cerr << "Couldn't initialize TTF: " << SDL_GetError() << std::endl;
        exit( 1 );
    }
    atexit( TTF_Quit );

    srand( time( NULL) );

    SDL_WM_SetCaption("Qonk - input not grabbed - press G to toggle", NULL);
    SDL_ShowCursor(SDL_DISABLE);

    Canvas::initScreen(Settings::getScreenWidth(), Settings::getScreenHeight(), Settings::getFullscreen());

    const int NUM_STICKS = SDL_NumJoysticks();
    sticks = new SDL_Joystick*[NUM_STICKS];
    for (int i=0;i<NUM_STICKS;i++)
        sticks[i] = SDL_JoystickOpen(i);

    initActionMap();
}

// Translates the input map into an action map where GameAction
// values can be looked up more quickly.
void
SDLDriver::initActionMap()
{
  actionMap.clear();

  for (int i=GA_FIRST; i<GA_COUNT;i++)
  {
    GameAction ga = (GameAction) i;

    Input input = Settings::getInput(ga); 
    pair< int, int> k = key(input.inputType, input.id0, input.id1, input.id2);

    if (input.inputType != IT_NONE)
      actionMap[k] = ga;

    input = Settings::getAltInput(ga);
    k = key(input.inputType, input.id0, input.id1, input.id2);

    if (input.inputType != IT_NONE)
      actionMap[k] = ga;
  }

}

SDLDriver::~SDLDriver()
{
  SDL_ShowCursor(SDL_ENABLE);

  delete sticks;

  Canvas::shutdown();

  SDL_Quit();
}

void
SDLDriver::setSenseMode(bool b)
{
  if (b)
    sensedInput = new Input();
  else
  {
    delete sensedInput;
    sensedInput = 0;
  }
}

Input &
SDLDriver::getSensedInput()
{
  return *sensedInput;
}

inline pair <int, int>
SDLDriver::key(InputType it, int id0, int id1, int id2)
{
  return pair< int, int> (it << 24 | id0, id1 << 16 | id2);
}

void
SDLDriver::input(InputType type, int id0, int id1, int id2, int value)
{
  GameAction ga = actionMap[key(type, id0, id1, id2)];

  if (sensedInput)
  {
    if (ga == GA_TOGGLE_GRAB)
    {
      if (!value)
        toggleGrab();

      return;
    }

    if (ga == GA_LEAVE)
    {
      inputHandler.handle(GA_SENSE_CANCEL, value);
      return;
    }

    if (!value)
    {
      sensedInput->inputType = type;
      sensedInput->id0 = id0;
      sensedInput->id1 = id1;
      sensedInput->id2 = id2;

      inputHandler.handle(GA_SENSE_COMPLETE, 0);

      return;
    }

  }
  
  if (ga != GA_NULL)
  {
    if (ga == GA_TOGGLE_GRAB && value)
      toggleGrab();
    else if (ga)
      inputHandler.handle(ga, value);
  }
}

void
SDLDriver::toggleGrab()
{
  if (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON)
  {
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_WM_SetCaption("Qonk - input not grabbed - press G to toggle", NULL);
  }
  else
  {
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_WM_SetCaption("Qonk - input grabbed - press G to toggle", NULL);
  }

}

//-----------------------------------------------------------------------------
void
SDLDriver::loop()
{
    static MouseState mouseXPos = INITIAL, mouseXNeg = INITIAL;
    static MouseState mouseYPos = INITIAL, mouseYNeg = INITIAL;

    SDL_Event ev;

    while(SDL_PollEvent(&ev))
    {
        switch(ev.type)
        {
        case SDL_QUIT:
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            //std::cerr << "key: " << SDL_GetKeyName(ev.key.keysym.sym) << (ev.key.state ? " pressed" : " released") << std::endl;

            // Prevents the menu system from doing anything as long as the input sensing is at work.
            // We want this because otherwise the menu system may act upon pressing of enter, escape
            // or such.
            if (!sensedInput)
              sdlInput->pushInput(ev);

            input(IT_KEYBOARD, ev.key.keysym.sym, 0, 0, ev.key.state<<2);
            break;
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            input(IT_MOUSEBUTTON, ev.button.button, 0, 0, ev.button.state<<2);
            break;
        case SDL_MOUSEMOTION:
            // Encoding of the id fields for the mousemotion is
            // id0: x axis = 0; y axis = 1
            // id1: positive movement = AD_POSITIVE; negative movement = AD_NEGATIVE
            // id2: unused
            // value: relative movement (always a positive value)
            
            if (ev.motion.xrel <= -DEADZONE_MOUSE)
            {
                input(IT_MOUSEMOTION, 0, AD_NEGATIVE, 0, -ev.motion.xrel);
                mouseXNeg = MOVED;
            }
            else if(ev.motion.xrel >= DEADZONE_MOUSE)
            {                
                input(IT_MOUSEMOTION, 0, AD_POSITIVE, 0, ev.motion.xrel);
                mouseXPos = MOVED;
            }

            if (ev.motion.yrel <= -DEADZONE_MOUSE)
            {
                input(IT_MOUSEMOTION, 1, AD_NEGATIVE, 0, -ev.motion.yrel);
                mouseYNeg = MOVED;
            }
            else if(ev.motion.yrel >= DEADZONE_MOUSE)
            {                
                input(IT_MOUSEMOTION, 1, AD_POSITIVE, 0, ev.motion.yrel);
                mouseYPos = MOVED;
            }

            break;
        case SDL_JOYAXISMOTION:
            if(ev.jaxis.value <= -DEADZONE_STICK)
                input(IT_STICKMOTION, ev.jaxis.which, ev.jaxis.axis, AD_NEGATIVE, -ev.jaxis.value>> 12);
            else if(ev.jaxis.value >= DEADZONE_STICK)
                input(IT_STICKMOTION, ev.jaxis.which, ev.jaxis.axis, AD_POSITIVE, ev.jaxis.value >> 12);
            else
              {
                input(IT_STICKMOTION, ev.jaxis.which, ev.jaxis.axis, AD_NEGATIVE, 0);
                input(IT_STICKMOTION, ev.jaxis.which, ev.jaxis.axis, AD_POSITIVE, 0);
              }

            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            input(IT_STICKBUTTON, ev.jbutton.which, ev.jbutton.button, 0,
                  ev.jbutton.state << 3);
            break;
        }  // switch
    }   // while (SDL_PollEvent())

    // If mouse has not moved in this input cycle an extra - axis stand still -
    // input event is transmitted. This makes the relative mouse axis appear more
    // like a key or joystick axis where releases and non-movement can be detected.
    if (mouseXNeg == RESET_NEEDED)
    {
      input(IT_MOUSEMOTION, 0, AD_NEGATIVE, 0, 0);
      mouseXNeg = INITIAL;
    }
    else if (mouseXNeg == MOVED)
      mouseXNeg = RESET_NEEDED;

    if (mouseXPos == RESET_NEEDED)
    {
      input(IT_MOUSEMOTION, 0, AD_POSITIVE, 0, 0);
      mouseXPos = INITIAL;
    }
    else if (mouseXPos == MOVED)
      mouseXPos = RESET_NEEDED;

    if (mouseYNeg == RESET_NEEDED)
    {
      input(IT_MOUSEMOTION, 1, AD_NEGATIVE, 0, 0);
      mouseYNeg = INITIAL;
    }
    else if (mouseYNeg == MOVED)
      mouseYNeg = RESET_NEEDED;

    if (mouseYPos == RESET_NEEDED)
    {
      input(IT_MOUSEMOTION, 1, AD_POSITIVE, 0, 0);
      mouseYPos = INITIAL;
    }
    else if (mouseYPos == MOVED)
      mouseYPos = RESET_NEEDED;

}

gcn::SDLInput *
SDLDriver::getSDLInput()
{
  return sdlInput;
}
