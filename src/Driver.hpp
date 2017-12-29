#pragma once

#include <memory>
#include <string>
#include <SDL.h>

namespace pix {
class Subject;
}

class Driver
{
  SDL_Window*                   window   = nullptr;
  SDL_Renderer*                 renderer = nullptr;
  SDL_Texture*                  texture  = nullptr;
  std::unique_ptr<pix::Subject> subject;

public:
  Driver(unsigned w, unsigned h, const char* filename = nullptr);
  ~Driver();
  /**
   * @brief Handles the event.
   * @return true if it need to be redrawn;
   */
  bool handle(const SDL_Event& ev);
  void render() const;

private:
  void        redraw() const;
  std::string inputCommand(const std::string& prompt) const;
};
