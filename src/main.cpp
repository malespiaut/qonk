// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifdef HAVE_CONFIG_H
# include "config.h"
#else
#define VERSION no-version
#endif

#include <sstream>
#include <string>

#include "sdl_driver.h"
#include "settings.h"

#include "engine/canvas.h"

#include "engine/game.h"
#include "main.h"

#include "menumanager.h"
#include "videooptions.h"
#include "gameoptions.h"

using namespace std;

Main::Main()
  : lastTicks(0), fpsCounter(0), fps(0), game(0), running(true) 
{
  driver = new SDLDriver(*this);
  menuManager = new MenuManager(*this);
}

Main::~Main()
{
  delete menuManager;
  delete driver;
}

void
Main::startSinglePlayerGame(int planets, int players)
{
  GameOptions &gameOptions = menuManager->getGameOptions();

  if( planets <= 1 )
  {
    cerr << "Resetting to least of 2 planets" << endl;
    planets = 2;
  }

  if( players > planets - 1 )
  {
    cerr << "Resetting computer players to maximum of " << planets - 1 << endl;
    players = planets - 1;
  }

  if( players < 1 )
  {
    cerr << "Resetting computer players to at least 1" << endl;
    players = 1;
  }

  gameOptions.setPlanets(planets);
  gameOptions.setPlayers(players);

  startSinglePlayerGame(gameOptions);
}

void
Main::startSinglePlayerGame(GameOptions &gameOptions)
{
  if (game)
  {
    delete game;
    game = 0;
  }

  menuManager->hide();

  game = new Game(gameOptions);
  game->setPaused(false);
}

void
Main::quit()
{
  running = false;
}

void
Main::showMenu()
{
  if (menuManager->isVisible())
    return;

  menuManager->show();

  // Just in case there is a game, pause it.
  if (game)
    game->setPaused(true);

}

void
Main::applyVideoOptions(VideoOptions &videoOptions)
{
      // Causes the game to pause which is nice when the CRT/TFT is resyncing.
      showMenu();

      bool f = videoOptions.getFullscreen();
      int w = videoOptions.getScreenWidth();
      int h = videoOptions.getScreenHeight();

      Settings::setFullscreen(f);
      Settings::setScreenSize(w, h);

      Canvas::initScreen(w, h, f);

      menuManager->resize();
}

void
Main::handle(GameAction gameAction, int value)
{
  switch (gameAction)
  {
    case GA_SENSE_CANCEL:
      if (!value)
        menuManager->senseFinished(false);
      break;
    case GA_SENSE_COMPLETE:
      if (!value)
        menuManager->senseFinished(true);
      break;
    case GA_TOGGLE_FULLSCREEN:
      if (value) return;
      {
        VideoOptions &vo = menuManager->getVideoOptions();
        vo.toggleFullscreen();
        applyVideoOptions(vo);
      }
      break;
    case GA_LEAVE:
      // It is important to act on keyrelease since this is the same
      // for what guichan reacts. By doing so it is predictable that
      // after handling GA_LEAVE (which may make the menusystem visible)
      // guichan's logic is called.
      if (value)
        return;

      // Main does not care about making the menu system invisible. This is done
      // from inside the menu system.
      if (!menuManager->isVisible())
        menuManager->show();

      break;
    case GA_SCREENSHOT:
      if (value)
        return;

      SDL_SaveBMP( SDL_GetVideoSurface(), "screenshot.bmp" );
      break;
    default:
      // Let the game instance handle the rest
      if (game)
        game->handle(gameAction, value);
      break;
  }
}

void
Main::renderFPS()
{
	stringstream s;
  
	fpsCounter++;

	s << fps;
	Canvas::drawText( Settings::getScreenWidth() - 32, 5, s.str().c_str(), 0x80, 0x80, 0x80 );

	if( SDL_GetTicks() - lastTicks >= 1000 ) {
		lastTicks = SDL_GetTicks();
    fps = fpsCounter;
		fpsCounter = 0;
	}
  
}

void
Main::run()
{
  bool b;

  // Main loop: loop forever.
	while (running) {

    Canvas::drawBox(0, 0, Settings::getScreenWidth(), Settings::getScreenHeight(), 0, 0, 0);

    if (game)
      game->render();
    menuManager->render();
    renderFPS();
    Canvas::updateScreen();
    
    b = menuManager->update();

    if (game)
    {
      if (!game->run(b))
      {
        delete game;
        game = 0;
        menuManager->showSinglePlayerMenu();
      }
    }

		// Poll for events, and handle the ones we care about.
    driver->loop();
	}

}

int main(int argc, char** argv) {
	int numberOfPlanets = -1;
	int numberOfComputerPlayers = -1;

  cout << "This is " << PACKAGE_STRING << endl;
  cout << "(c) 2003 Anthony Liekens" << endl;
  cout << "(c) 2007, 2008, 2009 Qonk development team" << endl;
  cout << "This game is free software released under the GNU General Public License, " << endl;
  cout << "either version 2 of the license or, at your option, any later version. " << endl;

	Settings::init();

	switch (argc) {
		case 3:
			numberOfPlanets = atoi( argv[ 1 ] );
			numberOfComputerPlayers = atoi( argv[ 2 ] );
			break;
    case 1:
      // Do nothing.
      break;
		default:
			cerr << "Usage: " << argv[ 0 ] << " numberOfPlanets numberOfComputerPlayers" << endl;
			exit(1);
			break;
	}

  Main *main = new Main();

  if (numberOfPlanets == -1
      && numberOfComputerPlayers == -1)
    main->showMenu();
  else
    main->startSinglePlayerGame(numberOfPlanets, numberOfComputerPlayers);

  main->run();
  
  delete main;

  Settings::store();

  return 0;
}

void
Main::startSensing()
{
  driver->setSenseMode(true);
}

void
Main::finishSensing(GameAction ga, bool completed)
{
  if (completed)
  {
    Settings::set(ga, driver->getSensedInput());
    driver->initActionMap();
  }

  driver->setSenseMode(false);
}

void
Main::resetGameAction(GameAction ga)
{
   Settings::unset(ga);
   driver->setSenseMode(true);
   driver->setSenseMode(false);
}

