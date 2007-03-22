// Copyright 2005 by Anthony Liekens anthony@liekens.net

#include "coordinate.h"
#include "stars.h"
#include "planets.h"
#include "selection.h"
#include "players.h"
#include "universe.h"
#include "fonts.h"
#include "ReadConf.h"
#include "config.h"

#include <SDL_gfxPrimitives.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <list>
using namespace std;

Font* font;
Uint32 FPStimer, FPScounter;
double FPS;

enum States { PLAYING, LOST, WON };
States state;

void render( SDL_Surface* screen, Universe* universe, Players* players ) {

	boxRGBA( screen, 0, 0, settings.getScreenWidth(), settings.getScreenHeight(), 0, 0, 0, 255 );
	
	universe->renderBackground( screen );
	players->render( screen );
	universe->renderForeground( screen );

	stringstream s;
	s << timer.getTimeMMSSHH();
	
	FPScounter++;
	if( SDL_GetTicks() - FPStimer >= 1000 ) {
		if( FPS == 0 )
			FPS = ( FPS + 1000.0 * FPScounter / ( SDL_GetTicks() - FPStimer ) ) / 2;
		else
			FPS = 1000.0 * FPScounter / ( SDL_GetTicks() - FPStimer );
		FPStimer = SDL_GetTicks();
		FPScounter = 0;
	}
	s << " (" << setiosflags(ios::fixed) << setprecision(2) << FPS << "FPS)";
	font->render( screen, 13, settings.getGameHeight() - 30, s.str().c_str(), 0x80, 0x80, 0x80 );

	SDL_UpdateRect(screen, 0, 0, settings.getScreenWidth(), settings.getScreenHeight() );
  
}

void changeScreen(SDL_Surface **screen) {

	if( settings.getFullscreen() )
		*screen = SDL_SetVideoMode( settings.getScreenWidth(), settings.getScreenHeight(), 32, SDL_SWSURFACE | SDL_FULLSCREEN );
	else
		*screen = SDL_SetVideoMode( settings.getScreenWidth(), settings.getScreenHeight(), 32, SDL_SWSURFACE );
}


int main(int argc, char** argv) {
	
/*	bool fullscreen = config.getValueBOOL("fullscreen");
	int h = config.getValueINT("height");
	int w = config.getValueINT("width");
	
	settings.setResolution( w, h );
	settings.setFullscreen( fullscreen );
*/	
	bool FirstRun = true;

	int numberOfPlanets;
	int numberOfComputerPlayers;
	
	bool NextRound = true;
	while (NextRound){
	state = PLAYING;
	
	if (FirstRun){
	FirstRun=false;
	if( argc != 3 ) {
		//cerr << "Usage: " << argv[ 0 ] << " numberOfPlanets numberOfComputerPlayers" << endl;
		//cerr << "Setting to default 6 planets and 1 computer player" << endl;
//		numberOfPlanets = config.getValueINT("numberOfPlanets");
//		numberOfComputerPlayers = config.getValueINT("numberOfComputerPlayers");
		numberOfPlanets = 6;
		numberOfComputerPlayers = 1;
	} else {
		numberOfPlanets = atoi( argv[ 1 ] );
		numberOfComputerPlayers = atoi( argv[ 2 ] );
	}
	if( numberOfPlanets <= 1 ) {
		cerr << "Resetting to least of 2 planets" << endl;
		numberOfPlanets = 2;
	}
	if( numberOfComputerPlayers > numberOfPlanets - 1 ) {
		cerr << "Resetting computer players to maximum of " << numberOfPlanets - 1 << endl;
		numberOfComputerPlayers = numberOfPlanets - 1;
	}
	if( numberOfComputerPlayers <= 1 ) {
		cerr << "Resetting computer players to at least 1" << endl;
		numberOfComputerPlayers = 1;
	}
	}
	// determine next level when this was passed successfully
	
	int nextPlanets = numberOfPlanets + 1;
	int nextComputerPlayers = numberOfComputerPlayers + 1;
	if( nextPlanets == 11 ) {
		int d = nextPlanets - nextComputerPlayers;
		if( d != 1 ) {
			nextPlanets = 10;
		}
	}

	/* Init SDL */
	if( SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO ) < 0 ) {
		cerr << "Couldn't initialize SDL: " << SDL_GetError() << endl;
		exit( 1 );
	}
	timer.start();
	atexit( SDL_Quit );
	if ( TTF_Init() < 0 ) {
		cerr << "Couldn't initialize TTF: " << SDL_GetError() << endl;
		exit( 1 );
	}
	atexit( TTF_Quit );
	font = new Font( "font.ttf", 18 );
	
	srand( time( NULL) );
	
	/* Set window title */
	SDL_WM_SetCaption( "Qonk", "Qonk" );

	/* Initialize the display */
	SDL_Surface *screen;
//	cout << settings.getGameWidth() << "+" << settings.getGameOffsetX() << endl;
	
	changeScreen(&screen);
	
	Universe* universe = new Universe( numberOfPlanets );
	
	Players* players = new Players( universe );
	universe->actionQueue->scheduleAction( 1000, new UpdatePlayersStatsAction( players ) );

	Planets::iterator planetIterator = universe->planets->begin();
	HumanPlayer* humanPlayer = new HumanPlayer( universe, &(*planetIterator), 3, 0xffffff );
	players->push_back( humanPlayer );
	planetIterator++;
	
	int hueCounter = rand() % 360;
	int counter = 0;
	do {
		if( counter < numberOfComputerPlayers ) {
			players->push_back( new ComputerPlayer( universe, &(*planetIterator), 3, HSVtoRGB( hueCounter, 0.9, 0.9 ) ) );
			counter++;
			hueCounter = ( hueCounter + 360 / numberOfComputerPlayers ) % 360;
		} else
			if( !(&(*planetIterator))->getMoon() )
				players->push_back( new NeutralPlayer( universe, &(*planetIterator), 1 + rand() % 3, 0x606060 ) );
			else
				players->push_back( new NeutralPlayer( universe, &(*planetIterator), 1 + rand() % 2, 0x606060 ) );
		planetIterator++;
	} while( planetIterator != universe->planets->end() );
	

	Selection selection;
	
	int iteration = 0;

	bool showEnemyShips = true;
	
	bool quit = false;

	// Main loop: loop forever.
	while( !quit ) {
	
		render( screen, universe, players );
		
		if( !timer.isPaused() ) {

			// update the universe
		
			universe->update();

			// let players update their states
		
			players->update();
			
		}
		
		// Poll for events, and handle the ones we care about.
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		
			switch (event.type) {
			
				case SDL_KEYDOWN:
				break;
				case SDL_KEYUP:
					if( event.key.keysym.sym == SDLK_p ) {
						timer.pause();
					}
//					if( event.key.keysym.sym == SDLK_t )
//						showEnemyShips = !showEnemyShips;
					// If escape is pressed, return (and thus, quit)
					if(event.key.keysym.sym == SDLK_ESCAPE){
						quit = true;
						NextRound = false;
					}
					if(event.key.keysym.sym == SDLK_n)
 						if (state==WON)
 							quit = true;
 						
 					if(event.key.keysym.sym == SDLK_r){
 						quit = true;
 						nextPlanets=numberOfPlanets;
						nextComputerPlayers = numberOfComputerPlayers;
 					}
 					if(event.key.keysym.sym == SDLK_f){
 						if(settings.getFullscreen()){
 							settings.setFullscreen( false );
 						}
 						else if(!settings.getFullscreen()){
 							settings.setFullscreen( true );
 						}
 							
 						changeScreen(&screen);
 					}
				break;
				case SDL_QUIT:
					quit = true;
					NextRound = false;
				
			}
			
		}
		
		if( humanPlayer->getPoints() == 0 ) {
			showEnemyShips = true;
			state = LOST;
		}
		
		if( state == PLAYING ) {
			bool allPlayersDead = true;
			for( Players::iterator i = players->begin(); i != players->end(); i++ ) {
				if( (*i)->getPlayerType() == Player::COMPUTER )
					if( ((ComputerPlayer*)(*i))->getPoints() > 0 )
						allPlayersDead = false;
			}
			if( allPlayersDead == true ) {
				MSGwon(universe, nextPlanets, nextComputerPlayers);
				state = WON;
			}
		}
		
		Uint8 *keystate = SDL_GetKeyState(NULL);
		bool show = false;
		if( keystate[SDLK_e] ) {
			show = true;
		}
		if( keystate[SDLK_a] ) {
			humanPlayer->selectAllPlanets();
		}
		if( keystate[SDLK_s] ) {
			SDL_SaveBMP( SDL_GetVideoSurface(), "screenshot.bmp" );
		}
		for ( int i=SDLK_0; i<=SDLK_9; i++ ) {
		  if (keystate[i]) {
		    int sel = (i == SDLK_0)
		              ? 100
			      : (i-SDLK_0) * 10;
		    humanPlayer->setFleetSelection(sel);
		  }
		}
		
		for( Players::iterator i = players->begin(); i != players->end(); i++ ) {
			if( (*i)->getPlayerType() == Player::COMPUTER )
				((ComputerPlayer*)(*i))->setDisplayShips( show || showEnemyShips );
			if( (*i)->getPlayerType() == Player::NEUTRAL )
				((NeutralPlayer*)(*i))->setDisplayShips( show || showEnemyShips );
		}
		
		// Keep shut for some time, not to use too much CPU
		
		// SDL_Delay( 20 );
		
	}
	
	delete font;
	delete universe;
	delete players;
	
	/*  // We dont want to much output to cout
	if( state == LOST ) {
		cout << "The AI players have kicked you out? Bastards!" << endl;
		cout << "Try harder, run QONK again, with \n\n\t" << argv[ 0 ] << " " << numberOfPlanets << " " << numberOfComputerPlayers << "\n" << endl;	
	} else if ( state == PLAYING ) {
		cout << "You gave up early?!" << endl;
		cout << "Try again, run QONK again, with \n\n\t" << argv[ 0 ] << " " << numberOfPlanets << " " << numberOfComputerPlayers << "\n" << endl; 
	} else {
		cout << "You have QONKuered the solar system! You won!" << endl;
		cout << "Move on to the next level, and run QONK again with \n\n\t" << argv[ 0 ] << " " << nextPlanets << " " << nextComputerPlayers << "\n" << endl;
	}*/
	
	numberOfPlanets = nextPlanets; 
 	numberOfComputerPlayers = nextComputerPlayers;
	}
	return 0;
	
}
