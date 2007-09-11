// menuaction.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef MENUACTION_H
#define MENUACTION_H

#include "guichan/guichan.hpp"

/* TODO: Realize all customized behavior with specific MenuEntry subclasses
 * and remove the need for a MenuAction delegate object.
 */
class MenuAction
{
  public:
    virtual gcn::Widget *getWidget(std::string);

    virtual void invoke() = 0;

    virtual void next() { };

    virtual void previous() { };

    virtual void update() { };

    /** Returns true if some operation was cancelled or false if not.
     */ 
    virtual bool cancel() { return false; }
    
    virtual void reset() { }
};

#endif
