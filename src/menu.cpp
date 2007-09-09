#include "menu.h"

#include <iostream>

#include "menu.h"
#include "menusystem.h"
#include "menuentry.h"
#include "menuaction.h"

using namespace gcn;
using namespace std;

Menu::Menu(Id parentId, std::string menuTitle, int x)
  : menuSystem(0), selected(-1), locx(x)
{
  parentMenuId = parentId; 
  container = new Container();

  title = new Label(menuTitle);
  container->add(title);
  container->setOpaque(false);
  container->setVisible(false);

  entries = new vector < MenuEntry * >();
}

Menu::~Menu()
{
  for (vector< MenuEntry * >::iterator i = entries->begin(); i != entries->end(); i++)
  {
    delete (*i);
  }

  delete entries;

  delete container;
}

void
Menu::addEntry(MenuEntry *entry)
{
  entries->push_back(entry);

  container->add(entry->getWidget());

  if (menuSystem)
    entry->registered(menuSystem);
}

Container *
Menu::getContainer() const
{
  return container;
}

void
Menu::enter()
{
  container->setVisible(true);

  if (selected == -1)
  {
    selected = 0;
    entries->at(0)->getWidget()->setFont(menuSystem->getHighlightedFont());
  }
}

void
Menu::leave()
{
  container->setVisible(false);
}

void
Menu::registered(MenuSystem *system)
{
  menuSystem = system;

  for (vector< MenuEntry * >::iterator i = entries->begin(); i != entries->end(); i++)
    (*i)->registered(system);

}

void
Menu::resize(int w, int h)
{
  container->setSize(w, h);
  
  int maxWidth = 0;
  for (vector< MenuEntry * >::iterator i = entries->begin(); i != entries->end(); i++)
  {
    gcn::Widget *w = (*i)->getWidget();
    maxWidth = max(w->getWidth(), maxWidth);
  }

  int x = (!locx) ? (w/2 - maxWidth/2) : locx;
  int y = h/8;

  title->setPosition(x, y);
  y += 2 * title->getHeight();

  for (vector< MenuEntry * >::iterator i = entries->begin(); i != entries->end(); i++)
  {
    gcn::Widget *w = (*i)->getWidget();
    w->setPosition(x, y);
    y += w->getHeight();
  }

}

void
Menu::up()
{
  entries->at(selected)->getWidget()->setFont(menuSystem->getNormalFont());

  if (--selected < 0)
    selected = entries->size()-1;

  entries->at(selected)->getWidget()->setFont(menuSystem->getHighlightedFont());
}

void
Menu::down()
{
  entries->at(selected)->getWidget()->setFont(menuSystem->getNormalFont());

  if (++selected >= entries->size())
    selected = 0;

  entries->at(selected)->getWidget()->setFont(menuSystem->getHighlightedFont());
}

void
Menu::cancel()
{
  // If the cancel operation aborted some operation we skip going one
  // menu level up.
  if (!entries->at(selected)->cancel())
  {
    entries->at(selected)->getWidget()->setFont(menuSystem->getNormalFont());
    selected = 0;
    entries->at(selected)->getWidget()->setFont(menuSystem->getHighlightedFont());

    menuSystem->enter(parentMenuId);
    
    cerr << "nothing was canceled" << endl;
  }
}

void
Menu::reset()
{
  entries->at(selected)->reset();
}

void
Menu::invoke()
{
  entries->at(selected)->invoke();
}

void
Menu::next()
{
  entries->at(selected)->next();
}

void
Menu::previous()
{
  entries->at(selected)->previous();
}

class ActionEntry : public MenuEntry
{
  MenuAction *action;

  public:
    ActionEntry(string s, MenuAction *a)
      : action(a), MenuEntry(a->getWidget(s))
    {
    }

    ~ActionEntry()
    {
      delete action;
    }

    void invoke()
    {
      action->invoke();
    }

    void previous()
    {
      action->previous();
    }
    
    void next()
    {
      action->next();
    }

    /** Delegates the question if some action was
     * cancelled or not to the action.
     */
    bool cancel()
    {
      return action->cancel();
    }
    
    void reset()
    {
      action->reset();
    }

};

void
Menu::addAction(string s, MenuAction *action)
{
  addEntry(new ActionEntry(s, action));
}

class LinkEntry : public MenuEntry
{
    Menu::Id target;

  public:
    LinkEntry(string s, Menu::Id id)
      : MenuEntry(new Label(s)), target(id)
    {
    }

    void invoke()
    {
        menuSystem->enter(target);
    }

};

void
Menu::addLink(string s, Menu::Id id)
{
  addEntry(new LinkEntry(s, id));
}

class BackLink : public MenuEntry
{
  Menu *menu;

  public:
    BackLink(Menu *newMenu)
      : MenuEntry(new Label("back")), menu(newMenu)
    {
    }

    void invoke()
    {
        menu->cancel();
    }

};

void
Menu::addBackLink()
{
  addEntry(new BackLink(this));
}

