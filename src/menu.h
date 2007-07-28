// menu.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef MENU_H
#define MENU_H

#include <vector>

#include "guichan/guichan.hpp"

class MenuSystem;
class MenuEntry;
class MenuAction;

class Menu
{
  public:
  enum Id
  {
    NONE, MAIN, SINGLEPLAYER, MULTIPLAYER, OPTIONS, AUDIO_OPTIONS, VIDEO_OPTIONS,
    CONTROL_OPTIONS
  };

  private:

  Id parentMenuId;

  gcn::Container *container;

  int selected;

  gcn::Label *title;

  std::vector < MenuEntry * > *entries;

  MenuSystem *menuSystem;

  int locx;

  public:
    Menu(Id, std::string, int = 0);

    ~Menu();

    void addEntry(MenuEntry *);

    void addAction(std::string, MenuAction *);

    void addLink(std::string, Menu::Id);

    void addBackLink();

    void registered(MenuSystem *);

    void enter();

    void leave();

    gcn::Container *getContainer() const;

    void resize(int, int);

    void up();

    void down();

    void cancel();

    void invoke();

    void next();

    void previous();

};

#endif
