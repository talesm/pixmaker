#include "Driver.hpp"
#include <iostream>
#include <unordered_map>
#include <SDL.h>
#include "BrushTool.hpp"
#include "CommandInput.hpp"
#include "LineTool.hpp"
#include "RectangleTool.hpp"
#include "Subject.hpp"

using namespace std;

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

static unique_ptr<pix::Tool>
chooseTool(string toolName)
{
  static auto brushToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::BrushTool>();
  };
  static auto lineToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::LineTool>(true);
  };
  static auto singleLineToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::LineTool>(false);
  };
  static auto rectToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::RectangleTool>(pix::RectangleTool::WIRED, false);
  };
  static auto filledRectToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::RectangleTool>(pix::RectangleTool::FILLED, false);
  };
  static auto squareToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::RectangleTool>(pix::RectangleTool::WIRED, true);
  };
  static auto squareRectToolGenerator = []() -> unique_ptr<pix::Tool> {
    return make_unique<pix::RectangleTool>(pix::RectangleTool::FILLED, true);
  };
  using ToolGenerator = std::function<unique_ptr<pix::Tool>()>;
  static unordered_map<string, ToolGenerator> toolGenerators{
    {"l", lineToolGenerator},
    {"ml", lineToolGenerator},
    {"sl", singleLineToolGenerator},
    {"line", lineToolGenerator},
    {"multiline", lineToolGenerator},
    {"singleline", singleLineToolGenerator},
    {"p", brushToolGenerator},
    {"pencil", brushToolGenerator},
    {"brush", brushToolGenerator},
    {"r", rectToolGenerator},
    {"wr", rectToolGenerator},
    {"rectangle", rectToolGenerator},
    {"wired rectangle", rectToolGenerator},
    {"fr", filledRectToolGenerator},
    {"filled rectangle", filledRectToolGenerator},
    {"s", squareToolGenerator},
    {"ws", squareToolGenerator},
    {"square", squareToolGenerator},
    {"wired square", squareToolGenerator},
    {"fs", squareRectToolGenerator},
    {"filled square", squareRectToolGenerator},
  };
  if (toolName == "") {
    throw runtime_error("Empty Tool name");
  }
  auto it = toolGenerators.find(toolName);
  if (it == toolGenerators.end()) {
    throw runtime_error("Invalid tool name: " + toolName);
  }
  return it->second();
}

EventResult
Driver::handle(const SDL_Event& ev)
{
  bool dirty = false;
  switch (ev.type) {
    case SDL_QUIT:
      return EventResult::CLOSE_WINDOW;
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
      } else if (ev.window.event == SDL_WINDOWEVENT_CLOSE) {
        return EventResult::CLOSE_WINDOW;
      }
      dirty = true;
      break;
    case SDL_KEYDOWN:
      if (ev.key.keysym.sym == SDLK_TAB) {
        auto commandInput = make_unique<CommandInput>(renderer, window);
        auto command      = commandInput->input("Type a command");
        if (command.size() > 0) {
          if (command[0] == 's') {
            if (command.size() == 1) {
              subject->save("./test.png");
            } else {
              subject->save(command.substr(1).c_str());
            }
          } else if (command[0] == 'q') {
            return EventResult::CLOSE_WINDOW;
          } else if (command[0] == 'l') {
            subject = make_unique<pix::Subject>(
              pix::Subject::load(command.substr(1).c_str()));
            dirty = true;
          } else if (command[0] == 'c') {
            subject->source(pix::Source::FromColorName(command.substr(1)));
          } else if (command[0] == 't') {
            try {
              subject->tool(chooseTool(command.substr(1)));
            } catch (std::runtime_error& e) {
              cerr << e.what() << endl;
            }
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
  return dirty ? EventResult::DIRTY_WINDOW : EventResult::NOTHING;
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
