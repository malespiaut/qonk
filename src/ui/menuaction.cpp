#include "guichan/guichan.hpp"

#include "menuaction.h"

using namespace std;

gcn::Widget *
MenuAction::getWidget(string s)
{
  return new gcn::Label(s);
}
