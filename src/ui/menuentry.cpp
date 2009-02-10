#include "guichan.hpp"

#include "menuentry.h"
#include "menusystem.h"

using namespace gcn;
using namespace std;

MenuEntry::MenuEntry(gcn::Widget *w)
 : menuSystem(0), widget(w)
{

}

MenuEntry::~MenuEntry()
{
  delete widget;
}

void
MenuEntry::registered(MenuSystem *system)
{
  menuSystem = system;
}


