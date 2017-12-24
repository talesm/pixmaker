#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "Subject.hpp"

using namespace std;

void
redraw(SDL_Renderer* renderer, SDL_Texture* texture, pix::Subject& surface);

int
main(int argc, char** argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) < 0) {
    cerr << SDL_GetError() << endl;
    return 1;
  }
  atexit(SDL_Quit);

  auto window = SDL_CreateWindow("Pix",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 800,
                                 600,
                                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    cerr << SDL_GetError() << endl;
    return 1;
  }
  auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr) {
    cerr << SDL_GetError() << endl;
    return 1;
  }

  auto subject = pix::Subject::create(800, 600);
  auto texture = SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 800, 600);

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
          subject.save("./test.png");
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        dirty |=
          subject.clickDown(ev.button.x,
                            ev.button.y,
                            pix::Button(ev.button.button - SDL_BUTTON_LEFT));
        break;
      case SDL_MOUSEBUTTONUP:
        dirty |=
          subject.clickUp(ev.button.x,
                          ev.button.y,
                          pix::Button(ev.button.button - SDL_BUTTON_LEFT));
        break;
      case SDL_MOUSEMOTION:
        dirty |= subject.move(ev.motion.x, ev.motion.y);
        break;
    }
    if (dirty) {
      redraw(renderer, texture, subject);
    }
  }

  return 0;
}

void
redraw(SDL_Renderer* renderer, SDL_Texture* texture, pix::Subject& surface)
{
  SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
  SDL_RenderClear(renderer);
  surface.render([&](unsigned             w,
                     unsigned             h,
                     unsigned char        bpp,
                     unsigned             stride,
                     const unsigned char* pixels) {
    SDL_UpdateTexture(texture, nullptr, pixels, stride);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
  });
  SDL_RenderPresent(renderer);
}
