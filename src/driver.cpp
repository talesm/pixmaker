#include <cstdlib>
#include <iostream>
#include <SDL.h>

using namespace std;

void
redraw(SDL_Renderer* renderer);

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

  SDL_Event ev;
  while (SDL_WaitEvent(&ev)) {
    switch (ev.type) {
      case SDL_QUIT:
        return 0;
      case SDL_WINDOWEVENT:
        redraw(renderer);
        break;
    }
  }

  return 0;
}

void
redraw(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
