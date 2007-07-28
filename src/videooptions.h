// videooptions.h
//
// (c) Robert Schuster, 2007
//
// Licensed under GNU GPL version 2 or, at your option, any later version.

#ifndef VIDEOOPTIONS_H
#define VIDEOOPTIONS_H

#include <vector>

class Main;
class MenuAction;

/**
 * A VideoOptions object is to be modified through simple 'apply' (or invoke)
 * pattern methods. Those methods are provided through internal MenuAction
 * class of which object can be created for the menu system. The MenuAction
 * instances also update the graphical value representation.
 */
class VideoOptions
{
  bool fullscreen;

  int screenWidth, screenHeight;

  std::vector < std::pair < int, int > > sizes;
  int index;

  MenuAction *toggleFullscreenAction;
  MenuAction *screenSizeAction;
  MenuAction *applyVideoOptionsAction;

  public:
    VideoOptions();

    void previousScreenSize();
    void nextScreenSize();
    void toggleFullscreen();

    bool getFullscreen() const { return fullscreen; }
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
  
    MenuAction *getToggleFullscreenAction();

    MenuAction *getScreenSizeAction();

    MenuAction *getApplyVideoOptionsAction(Main &);
};

#endif
