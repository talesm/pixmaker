#pragma once

#include <memory>
#include <string>
#include <SDL.h>

namespace pix {
class Subject;
}

enum class EventResult
{
  NOTHING,
  DIRTY_WINDOW,
  CLOSE_WINDOW
};

class Driver
{
public:
  Driver(unsigned w, unsigned h, const char* filename = nullptr);
  ~Driver();
  /**
   * @brief Handles the event.
   * @return true if it need to be redrawn;
   */
  EventResult handle(const SDL_Event& ev);
  void        render() const;

private:
  SDL_Window*                   window   = nullptr;
  SDL_Renderer*                 renderer = nullptr;
  SDL_Texture*                  texture  = nullptr;
  std::unique_ptr<pix::Subject> subject;

  void redraw() const;
};
