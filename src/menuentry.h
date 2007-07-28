// menuentry.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef MENUENTRY_H
#define MENUENTRY_H

#include "guichan/guichan.hpp"

class MenuSystem;

class MenuEntry
{
  protected:
    gcn::Widget *widget;

    MenuSystem *menuSystem;

    MenuEntry(gcn::Widget *);

  public:
    virtual ~MenuEntry();

    void registered(MenuSystem *);

    gcn::Widget *getWidget() const { return widget; }

    virtual void invoke() = 0;

    virtual void next() { };

    virtual void previous() { };

    /** Returns true if some operation was cancelled or false if not.
     * 
     * This is neccessary for the menu to know whether it should go
     * on level up or not.
     */ 
    virtual bool cancel() { return false; }

};

#endif
