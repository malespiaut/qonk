#include <iostream>

#include "menusystem.h"
#include "menu.h"

using namespace gcn;
using namespace std;

MenuSystem::MenuSystem(SDLInput *input, SDLGraphics *graphics)
  : currentMenu(0)
{
  gui = new Gui();
  
  gui->setInput(input);
  gui->setGraphics(graphics);
  Image::setImageLoader(new SDLImageLoader());

  menuKeyListener = new MenuSystem::KeyListener(*this);
  gui->addGlobalKeyListener(menuKeyListener);

  normal = loadFont("normal.png");
  highlighted = loadFont("highlight.png");
  gcn::Widget::setGlobalFont(normal);

  top = new Container();
  top->setOpaque(false);
  top->setVisible(true);
  top->setPosition(0,0);

  gui->setTop(top);

  menus = new map< Menu::Id, Menu * > ();
}

MenuSystem::~MenuSystem()
{
  delete highlighted;
  delete normal;

  delete top;

  gui->removeGlobalKeyListener(menuKeyListener);
  delete menuKeyListener;

  delete gui;

  delete menus;
}

void
MenuSystem::render()
{
  gui->draw();
}

bool
MenuSystem::update()
{
  gui->logic();

  if (currentMenu != nextMenu)
  {
    if (currentMenu)
      currentMenu->leave();
    
    if (nextMenu)
      nextMenu->enter();

    currentMenu = nextMenu;
  }

  return currentMenu;
}

void
MenuSystem::resize(int w, int h)
{
  top->setSize(w, h);

  for (map< Menu::Id, Menu * >::iterator i = menus->begin(); i != menus->end(); i++)
  {
    i->second->resize(w, h);
  }

}

void
MenuSystem::addMenu(Menu::Id menuId, Menu *menu)
{
  (*menus)[menuId] = menu;
  menu->registered(this);
  top->add(menu->getContainer());
}

ImageFont *
MenuSystem::getNormalFont()
{
  return normal;
}

ImageFont *
MenuSystem::getHighlightedFont()
{
  return highlighted;
}

ImageFont *
MenuSystem::loadFont(string fileName)
{
  ImageFont *font = 0;
  std::string s = PKG_DATA_DIR "/";
  s += fileName;

  try
  {
    font = new ImageFont(s, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    return font;
  }
  catch (gcn::Exception &e)
  {
  }

  s = "data";
#ifdef WINDOWS
  s += "\\";
#else
  s += "/";
#endif
  s += fileName;

  try
  {
    font = new ImageFont(s, " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,!?-+/():;%&`'*#=[]\"");
    return font;
  }
  catch (gcn::Exception &e)
  {
  }

  cerr << "Could not load " << fileName << " from the following directories: " << endl
       << PKG_DATA_DIR << endl
       << "data" << endl;

  exit( 1 );

  return 0;
}

Image *
MenuSystem::loadImage(string fileName)
{
  Image *image = 0;
  std::string s = PKG_DATA_DIR "/";
  s += fileName;

  try
  {
    image = Image::load(s);
    return image;
  }
  catch (gcn::Exception &e)
  {
  }

  s = "data";
#ifdef WINDOWS
  s += "\\";
#else
  s += "/";
#endif
  s += fileName;

  try
  {
    image = Image::load(s);
    return image;
  }
  catch (gcn::Exception &e)
  {
  }

  cerr << "Could not load " << fileName << " from the following directories: " << endl
       << PKG_DATA_DIR << endl
       << "data" << endl;

  exit( 1 );

  return 0;
}

/**
 * Makes the menu with the given id visible and hides the currently visible
 * one (if there is one).
 *
 * If called with no argument the main menu is shown.
 *
 * If called with id == NONE the menus are made invisible.
 *
 * Actually the menusystem does not change to current visible menu at once. It
 * just marks which menu it should show next and completes the request when
 * update() is called next.
 * Doing so prevents the menusystem from hiding right after it was made visible
 * from main.
 * 
 */
void
MenuSystem::enter(Menu::Id id)
{
  if (id != Menu::NONE)
  {
    nextMenu = (*menus)[id];
  }
  else
  {
    nextMenu = 0;
  }

}

bool
MenuSystem::isVisible() const
{
  return currentMenu;
}

void
MenuSystem::invoke()
{
  if (currentMenu)
    currentMenu->invoke();
}

void
MenuSystem::cancel()
{
  if (currentMenu)
    currentMenu->cancel();

}

MenuSystem::KeyListener::KeyListener(MenuSystem &menuSystem)
 : system(menuSystem)
{
}

void
MenuSystem::KeyListener::keyReleased(KeyEvent &keyEvent)
{
  // MenuSystem does not handle making itself visible. This is done from main.
  if (!system.currentMenu)
    return;

  switch (keyEvent.getKey().getValue())
  {
    case Key::BACKSPACE:
        system.currentMenu->reset();
        break;
    case Key::ESCAPE:
        system.currentMenu->cancel();
        break;
    case Key::ENTER:
        system.currentMenu->invoke();
        break;
    case Key::UP:
        system.currentMenu->up();
        break;
    case Key::DOWN:
        system.currentMenu->down();
        break;
    case Key::LEFT:
        system.currentMenu->previous();
        break;
    case Key::RIGHT:
        system.currentMenu->next();
        break;
  }
}
