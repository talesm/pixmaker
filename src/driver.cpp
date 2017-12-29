#include "Driver.hpp"
#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "Subject.hpp"

using namespace std;

int
main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
    cerr << SDL_GetError() << endl;
    return 1;
  }
  atexit(SDL_Quit);

  try {
    Driver    driver{800, 600, (argc > 1 ? argv[argc - 1] : nullptr)};
    SDL_Event ev;
    while (SDL_WaitEvent(&ev)) {
      if (driver.handle(ev)) {
        driver.render();
      }
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    return 1;
  }

  return 0;
}

Driver::Driver(unsigned w, unsigned h, const char* filename)
{
  window = SDL_CreateWindow("Pix",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            800,
                            600,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    throw runtime_error(SDL_GetError());
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    throw runtime_error(SDL_GetError());
  }
  if (filename != nullptr) {
    subject = make_unique<pix::Subject>(pix::Subject::load(filename));
  } else {
    subject = make_unique<pix::Subject>(pix::Subject::create(w, h));
  }
  texture = SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
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

bool
Driver::handle(const SDL_Event& ev)
{
  bool dirty = false;
  switch (ev.type) {
    case SDL_QUIT:
      return 0;
    case SDL_WINDOWEVENT:
      if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        auto newW = ev.window.data1;
        auto newH = ev.window.data2;
        int  oldW, oldH;
        SDL_QueryTexture(texture, nullptr, nullptr, &oldW, &oldH);
        if (oldW != newW || oldH != newH) {
          texture = SDL_CreateTexture(renderer,
                                      SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      newW,
                                      newH);
        }
      }
      dirty = true;
      break;
    case SDL_KEYDOWN:
      if (ev.key.keysym.sym == SDLK_TAB) {
        auto command = inputCommand("Type a command");
        if (command.size() > 0) {
          if (command[0] == 's') {
            if (command.size() == 1) {
              subject->save("./test.png");
            } else {
              subject->save(command.substr(1).c_str());
            }
          } else if (command[0] == 'q') {
            return 0;
          } else if (command[0] == 'l') {
            subject = make_unique<pix::Subject>(
              pix::Subject::load(command.substr(1).c_str()));
            dirty = true;
          } else {
            cout << "Invalid command: \n" << command << endl;
          }
        }
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      dirty |=
        subject->clickDown(ev.button.x,
                           ev.button.y,
                           pix::Button(ev.button.button - SDL_BUTTON_LEFT));
      break;
    case SDL_MOUSEBUTTONUP:
      dirty |=
        subject->clickUp(ev.button.x,
                         ev.button.y,
                         pix::Button(ev.button.button - SDL_BUTTON_LEFT));
      break;
    case SDL_MOUSEMOTION:
      dirty |= subject->move(ev.motion.x, ev.motion.y);
      break;
    case SDL_DROPFILE:
      subject = make_unique<pix::Subject>(pix::Subject::load(ev.drop.file));
      dirty   = true;
      break;
  }
  return dirty;
}

void
Driver::render() const
{
  SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
  SDL_RenderClear(renderer);
  redraw();
  SDL_RenderPresent(renderer);
}

void
Driver::redraw() const
{
  subject->render([&](unsigned             w,
                      unsigned             h,
                      unsigned char        bpp,
                      unsigned             stride,
                      const unsigned char* pixels) {
    SDL_Rect rect{0, 0, int(w), int(h)};
    SDL_UpdateTexture(texture, &rect, pixels, stride);
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
  KW_Rect titlerect   = {10, 10, 280, 30};
  KW_Rect labelrect   = {10, 50, 280, 30};
  KW_Rect editboxrect = {10, 100, 280, 40};
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
  // KW_ReleaseSurface(driver, set);
  // KW_ReleaseRenderDriver(driver);
  return input;
}