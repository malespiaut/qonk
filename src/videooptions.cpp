#include <vector>
#include <sstream>

#include <SDL/SDL.h>

#include "guichan/guichan.hpp"

#include "settings.h"

#include "videooptions.h"
#include "menuaction.h"
#include "main.h"

using namespace std;
using namespace gcn;

VideoOptions::VideoOptions()
  : toggleFullscreenAction(0), screenSizeAction(0), applyVideoOptionsAction(0)
{
  fullscreen = Settings::getFullscreen();
  screenWidth = Settings::getScreenWidth();
  screenHeight = Settings::getScreenHeight();

  SDL_Rect **modes = SDL_ListModes(NULL, SDL_FULLSCREEN
                                         | SDL_HWSURFACE
                                         | SDL_SWSURFACE);

  /* Check if there are any modes available */
  if(!modes)
  {
    // No modes available. :(
  } 
  else if(modes == (SDL_Rect **)-1){
    // This means we can chose every display size we want (will never happen).
  }
  else
  {
    for (int i=0; modes[i]; ++i)
      sizes.push_back(pair <int, int> (modes[i]->w, modes[i]->h));

    // Nicely sort the entries.
    sort (sizes.begin(), sizes.end());
  }

  // Initializes index with the proper value denoting the current
  // screen size.
  index = 0;
  int size = sizes.size();
  for (int i=0; i<size; i++)
  {
    if (sizes[i].first == screenWidth
        && sizes[i].second == screenHeight)
    {
      index = i;
      break;
    }
  }

}

void
VideoOptions::toggleFullscreen()
{
  fullscreen = !fullscreen;

  if (toggleFullscreenAction)
    toggleFullscreenAction->update();
}

class ToggleFullscreenAction : public MenuAction
{
  VideoOptions &videoOptions;
  CheckBox *check;

  public:
    ToggleFullscreenAction(VideoOptions &vo)
      : videoOptions(vo) { }

    virtual gcn::Widget *getWidget(string s)
    {
      return check = new CheckBox(s, videoOptions.getFullscreen());
    }

    void invoke()
    {
      videoOptions.toggleFullscreen();
    }

    void previous()
    {
      invoke();
    }

    void next()
    {
      invoke();
    }

    void update()
    {
      check->setMarked(videoOptions.getFullscreen());
    }
};

MenuAction *
VideoOptions::getToggleFullscreenAction()
{
  if (!toggleFullscreenAction)
    toggleFullscreenAction = new ToggleFullscreenAction(*this);

  return toggleFullscreenAction;
}

void
VideoOptions::nextScreenSize()
{
  if (++index >= sizes.size())
    index = 0;

  screenWidth = sizes[index].first;
  screenHeight = sizes[index].second;

  if (screenSizeAction)
    screenSizeAction->update();
}

void
VideoOptions::previousScreenSize()
{
  if (--index < 0)
    index = sizes.size() - 1;

  screenWidth = sizes[index].first;
  screenHeight = sizes[index].second;

  if (screenSizeAction)
    screenSizeAction->update();
}

class ScreenSizeAction : public MenuAction
{
  VideoOptions &videoOptions;
  Label *label;
  string baseCaption;

  public:
    ScreenSizeAction(VideoOptions &vo)
      : videoOptions(vo) { }

  virtual gcn::Widget *getWidget(string s)
  {
    baseCaption = s;
    stringstream ss;
    ss << baseCaption;
    ss << " ";
    ss << videoOptions.getScreenWidth();
    ss << "x";
    ss << videoOptions.getScreenHeight();

    return label = new Label(ss.str());
  }

  void invoke()
  {
    next();
  }

  void next()
  {
    videoOptions.nextScreenSize();
  }

  void previous()
  {
    videoOptions.previousScreenSize();
  }

  void update()
  {
    stringstream ss;
    ss << baseCaption;
    ss << " ";
    ss << videoOptions.getScreenWidth();
    ss << "x";
    ss << videoOptions.getScreenHeight();

    label->setCaption(ss.str());
    label->adjustSize();
  }

};

MenuAction *
VideoOptions::getScreenSizeAction()
{
  if (!screenSizeAction)
    screenSizeAction = new ScreenSizeAction(*this);

  return screenSizeAction;
}

class ApplyVideoOptionsAction : public MenuAction
{
  Main &main;
  VideoOptions &videoOptions;

  public:
    ApplyVideoOptionsAction(Main &m, VideoOptions &vo)
      : main(m), videoOptions(vo) { }

    void invoke()
    {
      main.applyVideoOptions(videoOptions);
    }
};

MenuAction *
VideoOptions::getApplyVideoOptionsAction(Main &main)
{
  if (!applyVideoOptionsAction)
    applyVideoOptionsAction = new ApplyVideoOptionsAction(main, *this);
    
  return applyVideoOptionsAction;
}
