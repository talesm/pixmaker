#include <cstdlib>
#include <iostream>
#include <SDL.h>
#include "Driver.hpp"

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
      switch (driver.handle(ev)) {
        case EventResult::CLOSE_WINDOW:
          return EXIT_SUCCESS;
          break;
        case EventResult::DIRTY_WINDOW:
          driver.render();
          break;
        case EventResult::NOTHING:
          break;
      }
    }
  } catch (exception& e) {
    cerr << e.what() << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}