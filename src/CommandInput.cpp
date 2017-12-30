#include "CommandInput.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Driver.hpp"

using namespace std;

static const int W = 400, H = 50;

CommandInput::CommandInput(SDL_Window* parentWindow)
  : parentWindow(parentWindow)
{
  TTF_Init();
  font = TTF_OpenFont("res/FreeMono.ttf", 16);
}

CommandInput::~CommandInput()
{
  TTF_CloseFont(font);
}

string
CommandInput::input(const string& prompt) const
{

  auto window   = SDL_CreateWindow(prompt.c_str(),
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 W,
                                 H,
                                 SDL_WINDOW_SHOWN);
  auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  auto windowId = SDL_GetWindowID(window);
  auto parentId = SDL_GetWindowID(parentWindow);
  SDL_SetWindowModalFor(window, parentWindow);
  SDL_Texture* textTex = nullptr;

  SDL_Event ev;
  string    input;
  bool      dirty = true;
  bool      quit  = false;
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_StartTextInput();
  while (!quit && !SDL_QuitRequested() && SDL_WaitEvent(&ev)) {
    switch (ev.type) {
      case SDL_KEYDOWN:
        if (ev.key.keysym.sym == SDLK_ESCAPE) {
          input = "";
          quit  = true;
        } else if (ev.key.keysym.sym == SDLK_RETURN ||
                   ev.key.keysym.sym == SDLK_KP_ENTER) {
          quit = true;
        } else if (ev.key.keysym.sym == SDLK_BACKSPACE) {
          if (input.size() > 0) {
            input.pop_back();
            dirty = true;
          }
        }
        break;
      case SDL_WINDOWEVENT:
        if (ev.window.windowID == windowId) {
          switch (ev.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
              input = "";
              quit  = true;
              break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_SHOWN:
              dirty = true;
              break;
          }
        } else if (ev.window.windowID == parentId &&
                   (ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED ||
                    ev.window.event == SDL_WINDOWEVENT_ENTER)) {
          SDL_RaiseWindow(window);
        }
        break;
      case SDL_TEXTINPUT:
        input += ev.text.text;
        dirty = true;
        break;
    }
    if (dirty) {
      SDL_SetRenderDrawColor(renderer, 224, 224, 224, 255);
      SDL_RenderClear(renderer);
      if (input.size() > 0) {
        auto textSurface =
          TTF_RenderUTF8_Blended(font, input.c_str(), {0, 0, 0, 255});
        if (!textSurface) {
          throw runtime_error(SDL_GetError());
        }
        SDL_DestroyTexture(textTex);
        textTex = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect rect{
          0, H / 2 - textSurface->h / 2, textSurface->w, textSurface->h};
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTex, nullptr, &rect);
      }
      SDL_RenderPresent(renderer);
    }
  }
  SDL_StopTextInput();
  SDL_DestroyTexture(textTex);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  /* free stuff */
  return input;
}
