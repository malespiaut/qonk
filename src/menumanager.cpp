#include "menumanager.h"
#include "menusystem.h"
#include "menu.h"

#include "menuaction.h"

#include "controloptions.h"
#include "gameoptions.h"
#include "videooptions.h"

#include "input.h"
#include "main.h"
#include "sdl_driver.h"

MenuManager::MenuManager(Main &newMain)
  : main(newMain)
{
  menuSystem = new MenuSystem(main.getSDLDriver().getSDLInput());

  controlOptions = new ControlOptions(main);
  videoOptions = new VideoOptions();
  gameOptions = new GameOptions();

  Menu *m = new Menu(Menu::NONE, "qonk");
  m->addLink("singleplayer", Menu::SINGLEPLAYER);
  m->addLink("multiplayer", Menu::MULTIPLAYER);
  m->addLink("options", Menu::OPTIONS);
  m->addAction("quit", new QuitAction(main));
  menuSystem->addMenu(Menu::MAIN, m);

  m = new Menu(Menu::MAIN, "singleplayer");
  m->addAction("planets", gameOptions->getPlanetsAction());
  m->addAction("enemies", gameOptions->getPlayersAction());
  m->addAction("show enemies", gameOptions->getEnemyVisibilityAction());
  m->addAction("start game", gameOptions->getStartSinglePlayerGameAction(main));
  m->addBackLink();
  menuSystem->addMenu(Menu::SINGLEPLAYER, m);

  m = new Menu(Menu::MAIN, "multiplayer");
  m->addLink("this is not", Menu::NONE);
  m->addLink("yet implemented.", Menu::NONE);
  m->addBackLink();
  menuSystem->addMenu(Menu::MULTIPLAYER, m);

  m = new Menu(Menu::MAIN, "options");
  m->addLink("audio", Menu::AUDIO_OPTIONS);
  m->addLink("video", Menu::VIDEO_OPTIONS);
  m->addLink("controls", Menu::CONTROL_OPTIONS);
  m->addBackLink();
  menuSystem->addMenu(Menu::OPTIONS, m);

  m = new Menu(Menu::OPTIONS, "audio");
  m->addLink("this is not", Menu::NONE);
  m->addLink("yet implemented.", Menu::NONE);
  m->addBackLink();
  menuSystem->addMenu(Menu::AUDIO_OPTIONS, m);

  m = new Menu(Menu::OPTIONS, "video");
  m->addAction("screen size:", videoOptions->getScreenSizeAction());
  m->addAction("toggle fullscreen", videoOptions->getToggleFullscreenAction());
  m->addAction("apply", videoOptions->getApplyVideoOptionsAction(main));
  m->addBackLink();
  menuSystem->addMenu(Menu::VIDEO_OPTIONS, m);

  m = new Menu(Menu::OPTIONS, "controls", 10);
  m->addAction("select all", controlOptions->getControlAction(GA_SELECT_ALL));
  m->addAction("show enemies", controlOptions->getControlAction(GA_TOGGLE_ENEMY_VISIBILITY));
  m->addAction("selection", controlOptions->getControlAction(GA_SELECTION));
  m->addAction("cursor up", controlOptions->getControlAction(GA_CURSOR_UP));
  m->addAction("cursor down", controlOptions->getControlAction(GA_CURSOR_DOWN));
  m->addAction("cursor left", controlOptions->getControlAction(GA_CURSOR_LEFT));
  m->addAction("cursor right", controlOptions->getControlAction(GA_CURSOR_RIGHT));
  m->addAction("select nearest", controlOptions->getControlAction(GA_SELECT_NEAREST_PLANET));
  m->addAction("move to nearest", controlOptions->getControlAction(GA_MOVE_TO_NEAREST_PLANET));
  m->addAction("single ship mode", controlOptions->getControlAction(GA_SET_FLEET_STRENGTH_SINGLE));
  m->addBackLink();
  menuSystem->addMenu(Menu::CONTROL_OPTIONS, m);

  menuSystem->resize();

  menuSystem->enter();
}

MenuManager::~MenuManager()
{
  delete menuSystem;

  delete gameOptions;
  delete videoOptions;
}

void
MenuManager::showSinglePlayerMenu()
{
  menuSystem->enter(Menu::SINGLEPLAYER);
}

void
MenuManager::show()
{
  menuSystem->enter();
}

void
MenuManager::hide()
{
  menuSystem->enter(Menu::NONE);
}

bool
MenuManager::isVisible() const
{
  return menuSystem->isVisible();
}

void
MenuManager::resize()
{
  menuSystem->resize();
}

void
MenuManager::render()
{
  menuSystem->render();
}

bool
MenuManager::update()
{
  return menuSystem->update();
}

void
MenuManager::senseFinished(bool b)
{
  if (b)
    menuSystem->invoke();
  else
    menuSystem->cancel();
}

