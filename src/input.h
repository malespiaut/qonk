// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef INPUT_H
#define INPUT_H

enum InputType
{
        IT_NONE,
        IT_KEYBOARD,
        IT_MOUSEMOTION,
        IT_MOUSEBUTTON,
        IT_STICKMOTION,
        IT_STICKBUTTON,
        IT_STICKHAT // TODO: Not yet implemented.  
};

enum AxisDirection
{
        AD_NEGATIVE,
        AD_POSITIVE
};

struct Input
{
        InputType inputType;

        int id0;
        int id1;
        int id2;
};

enum GameAction
{
        GA_NULL,                // Nothing. Dummy entry.

        GA_SENSE_CANCEL,        // Input mapping sense cancel.
        GA_SENSE_COMPLETE,      // Input mapping sense complete.

        GA_TOGGLE_GRAB,         // Toggles input grabbing for windowed mode

        // General actions 
        GA_LEAVE,               // Leave menu, game whatever.
        GA_SCREENSHOT,          // Make a screenshot.
        GA_PAUSE,               // Pause/Unpause the game.
        GA_TOGGLE_FULLSCREEN,   // Toggle between fullscreen and windowed mode.

        GA_RESTART_ROUND,       // Restart round.
        GA_NEXT_ROUND,          // Start next round.

        // Game related actions
        GA_SELECT_ALL,                  // Select all planets as source.
        GA_TOGGLE_ENEMY_VISIBILITY,       // Toggle between showing and hiding the enemy ships.

        GA_SELECTION,

        GA_CURSOR_UP,
        GA_CURSOR_DOWN,
        GA_CURSOR_LEFT,
        GA_CURSOR_RIGHT,

        GA_SELECT_NEAREST_PLANET,
        GA_MOVE_TO_NEAREST_PLANET,

        GA_SET_FLEET_STRENGTH_SINGLE,
        GA_FLEET_STRENGTH_UP,
        GA_FLEET_STRENGTH_DOWN,

        GA_SET_FLEET_STRENGTH_10,
        GA_SET_FLEET_STRENGTH_20,
        GA_SET_FLEET_STRENGTH_30,
        GA_SET_FLEET_STRENGTH_40,
        GA_SET_FLEET_STRENGTH_50,
        GA_SET_FLEET_STRENGTH_60,
        GA_SET_FLEET_STRENGTH_70,
        GA_SET_FLEET_STRENGTH_80,
        GA_SET_FLEET_STRENGTH_90,
        GA_SET_FLEET_STRENGTH_100,

};
#define GA_FIRST (GA_TOGGLE_GRAB)
#define GA_COUNT (GA_SET_FLEET_STRENGTH_100+1)

class InputHandler {

  public:
    virtual void handle(GameAction ga, int value) = 0;
};

#endif
