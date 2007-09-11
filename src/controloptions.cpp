#include <iostream>
#include <map>

#include "guichan/guichan.hpp"

#include "ui/menuaction.h"

#include "controloptions.h"
#include "main.h"
#include "settings.h"

#include "input.h"

using namespace std;
using namespace gcn;

ControlOptions::ControlOptions(Main &m)
  : main(m)
{
}

void
ControlOptions::startSensing()
{
  main.startSensing();
}

void
ControlOptions::finishSensing(GameAction ga, bool b)
{
  main.finishSensing(ga, b);
  
  map <GameAction, MenuAction * >::iterator iter;   
  for( iter = controlActions.begin(); iter != controlActions.end(); iter++ ) {
    iter->second->update();
  }
}

void
ControlOptions::reset(GameAction ga)
{
    main.resetGameAction(ga);
}

class ControlAction : public MenuAction
{
  ControlOptions &controlOptions;
  GameAction gameAction;
  bool sensing;

  Label *label;
  string base;

  public:
    ControlAction(ControlOptions &co, GameAction ga)
      : controlOptions(co), gameAction(ga), sensing(false), label(0)
    {
    }

    gcn::Widget *getWidget(string s)
    {
      base = s + ": ";
      return label = new Label(base + Settings::getAsString(gameAction));
    }

    void invoke()
    {
      if (sensing)
      {
        controlOptions.finishSensing(gameAction, true);
        sensing = false;
      }
      else
      {
        sensing = true;
        label->setCaption(base + "press something");
        label->adjustSize();
        controlOptions.startSensing();
      }
    }

    bool cancel()
    {
      if (sensing)
      {
        controlOptions.finishSensing(gameAction, false);
        sensing = false;
        
        // Report that an operation was interrupted (prevents
        // menu being left or menu system made invisible).
        return true;
      }
      
      return false;
    }
    
    void reset()
    {
      controlOptions.reset(gameAction);
      label->setCaption(base + Settings::getAsString(gameAction));
      label->adjustSize();
    }
    
    void update()
    {
      label->setCaption(base + Settings::getAsString(gameAction));
      label->adjustSize();
    }
    
};

MenuAction *
ControlOptions::getControlAction(GameAction ga)
{
  MenuAction *ma = controlActions[ga];
 
  if (!ma)
  {
    ma = new ControlAction(*this, ga);
    controlActions[ga] = ma;
  }

  return ma; 
}

