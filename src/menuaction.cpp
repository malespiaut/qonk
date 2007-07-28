#include "guichan/guichan.hpp"

#include "menuaction.h"
#include "main.h"

using namespace std;

gcn::Widget *
MenuAction::getWidget(string s)
{
  return new gcn::Label(s);
}

void
QuitAction::invoke()
{
  main.quit();
}
