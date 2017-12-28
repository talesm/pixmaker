#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "Subject.hpp"

using namespace std;

struct Driver
{
  SDL_Window*              window   = nullptr;
  SDL_Renderer*            renderer = nullptr;
  SDL_Texture*             texture  = nullptr;
  unique_ptr<pix::Subject> subject;

  ~Driver();
  void   redraw() const;
  string inputCommand(const string& prompt) const;
};

int
main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
    cerr << SDL_GetError() << endl;
    return 1;
  }
  atexit(SDL_Quit);

  Driver driver;
  driver.window = SDL_CreateWindow("Pix",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   800,
                                   600,
                                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  if (driver.window == nullptr) {
    cerr << SDL_GetError() << endl;
    return 1;
  }
  driver.renderer =
    SDL_CreateRenderer(driver.window, -1, SDL_RENDERER_ACCELERATED);
  if (driver.renderer == nullptr) {
    cerr << SDL_GetError() << endl;
    return 1;
  }

  driver.subject = make_unique<pix::Subject>(pix::Subject::create(800, 600));
  driver.texture = SDL_CreateTexture(driver.renderer,
                                     SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     800,
                                     600);

  SDL_Event ev;
  while (SDL_WaitEvent(&ev)) {
    bool dirty = false;
    switch (ev.type) {
      case SDL_QUIT:
        return 0;
      case SDL_WINDOWEVENT:
        dirty = true;
        break;
      case SDL_KEYDOWN:
        if (ev.key.keysym.sym == SDLK_TAB) {
          auto command = driver.inputCommand("Type a command");
          if (command.size() > 0) {
            if (command[0] == 's') {
              if (command.size() == 1) {
                driver.subject->save("./test.png");
              } else {
                driver.subject->save(command.substr(1).c_str());
              }
            } else if (command[0] == 'q') {
              return 0;
            } else {
              cout << "Invalid command: \n" << command << endl;
            }
          }
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        dirty |= driver.subject->clickDown(
          ev.button.x,
          ev.button.y,
          pix::Button(ev.button.button - SDL_BUTTON_LEFT));
        break;
      case SDL_MOUSEBUTTONUP:
        dirty |= driver.subject->clickUp(
          ev.button.x,
          ev.button.y,
          pix::Button(ev.button.button - SDL_BUTTON_LEFT));
        break;
      case SDL_MOUSEMOTION:
        dirty |= driver.subject->move(ev.motion.x, ev.motion.y);
        break;
    }
    if (dirty) {
      SDL_SetRenderDrawColor(driver.renderer, 127, 127, 127, 255);
      SDL_RenderClear(driver.renderer);
      driver.redraw();
      SDL_RenderPresent(driver.renderer);
    }
  }

  return 0;
}

Driver::~Driver()
{
  if (subject) {
    subject.reset();
  }
  if (texture) {
    SDL_DestroyTexture(texture);
  }
  if (renderer) {
    SDL_DestroyRenderer(renderer);
  }
  if (window) {
    SDL_DestroyWindow(window);
  }
}

void
Driver::redraw() const
{
  subject->render([&](unsigned             w,
                      unsigned             h,
                      unsigned char        bpp,
                      unsigned             stride,
                      const unsigned char* pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, stride);
    SDL_Rect rect{0, 0, 800, 600};
    SDL_RenderCopy(renderer, texture, &rect, &rect);
  });
}

#include "KW_button.h"
#include "KW_editbox.h"
#include "KW_frame.h"
#include "KW_gui.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"

/* Callback for when the OK button is clicked */
static bool quit = false;
static void
OKClicked(KW_Widget* widget, int b)
{
  quit = true;
}

static void
EnterPressed(KW_Widget* widget, SDL_Keycode sym, SDL_Scancode code)
{
  if (sym == SDLK_KP_ENTER || sym == SDLK_RETURN) {
    quit = true;
  }
}

string
Driver::inputCommand(const string& prompt) const
{
  quit = false;
  /* Initialize KiWi */
  KW_RenderDriver* driver = KW_CreateSDL2RenderDriver(renderer, window);
  KW_Surface*      set    = KW_LoadSurface(driver, "res/tileset.png");
  KW_GUI*          gui    = KW_Init(driver, set);

  /* Create the top-level framve */
  KW_Rect windowrect = {0, 0, 800, 600};
  KW_Rect framerect  = {10, 10, 300, 220};
  KW_RectCenterInParent(&windowrect, &framerect);
  KW_Widget* frame = KW_CreateFrame(gui, NULL, &framerect);

  /* Create the title, label and edibox widgets */
  KW_Rect  titlerect   = {10, 10, 280, 30};
  KW_Rect  labelrect   = {10, 50, 280, 30};
  KW_Rect  editboxrect = {10, 100, 280, 40};
  KW_Rect* rects[]     = {&labelrect, &editboxrect};
  // unsigned weights[]   = {1, 4};
  // KW_RectFillParentHorizontally(
  //   &framerect, rects, weights, 2, 10, KW_RECT_ALIGN_MIDDLE);
  KW_CreateLabel(gui, frame, "Input command", &titlerect);
  KW_CreateLabel(gui, frame, prompt.c_str(), &labelrect);

  auto editbox = KW_CreateEditbox(gui, frame, "", &editboxrect);
  KW_AddWidgetKeyUpHandler(editbox, EnterPressed);
  KW_SetFocusedWidget(editbox);

  KW_Rect    buttonrect = {250, 170, 40, 40};
  KW_Widget* okbutton = KW_CreateButtonAndLabel(gui, frame, "OK", &buttonrect);
  KW_AddWidgetMouseDownHandler(okbutton, OKClicked);

  /* Main loop */
  while (!SDL_QuitRequested() && !quit) {
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    redraw();
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  string input = KW_GetEditboxText(editbox);

  /* free stuff */
  KW_Quit(gui);
  KW_ReleaseSurface(driver, set);
  KW_ReleaseRenderDriver(driver);
  return input;
}