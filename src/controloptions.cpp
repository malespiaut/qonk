#include <iostream>
#include <map>

#include "guichan/guichan.hpp"

#include "controloptions.h"
#include "menuaction.h"
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
        label->setCaption(base + Settings::getAsString(gameAction));
        label->adjustSize();
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
        label->setCaption(base + Settings::getAsString(gameAction));
        label->adjustSize();
        sensing = false;
      }
      return sensing;
    }
    
    void reset()
    {
      controlOptions.reset(gameAction);
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

