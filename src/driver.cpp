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
  void redraw() const;
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
        if (ev.key.keysym.sym == SDLK_s) {
          driver.subject->save("./test.png");
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
      driver.redraw();
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
  SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
  SDL_RenderClear(renderer);
  subject->render([&](unsigned             w,
                      unsigned             h,
                      unsigned char        bpp,
                      unsigned             stride,
                      const unsigned char* pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, stride);
    SDL_Rect rect{0, 0, 800, 600};
    SDL_RenderCopy(renderer, texture, &rect, &rect);
  });
  SDL_RenderPresent(renderer);
}
